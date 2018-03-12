#include <fstream>
#include <iconv.h>
#include <boost/lexical_cast.hpp> //boost库中用于不同数据类型转换的
#include "crawler.h"


//这是一个回调函数
size_t WriteData(char *ptr, size_t size, size_t nmemb, void *userdata) {
  if (ptr == NULL || userdata == NULL) {
    return 0;//
  }
  std::string* str_ptr = (std::string*)userdata;
  str_ptr->append(ptr, size * nmemb);		//追加到ptr指向的缓冲区中，每次写入size*nmemb个字节大小的内容，
  return size * nmemb;
}

class ScopedHandle {
 public:
  ScopedHandle(CURL* handle) 
	: handle_(handle)
	{}
	
  ~ScopedHandle() 
  { 
	curl_easy_cleanup(handle_);
  }

 private:
  ScopedHandle(const ScopedHandle&);
  ScopedHandle& operator=(const ScopedHandle&);

  CURL* handle_;
};

bool GetHtml(const std::string& url, std::string* html) {
  CURL* handle = curl_easy_init();
  // 返回值是一个CURL*句柄
  
  ScopedHandle scoped_handle(handle);//用一个类来完成释放工作
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
 
  
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteData);
   // curl_easy_setopt（easyhandle，CURLOPT_WRITEFUNCTION，write_data）;
  //  可以通过设置另一个属性来控制回调函数在第四个参数中获取的数据：


  curl_easy_setopt(handle, CURLOPT_WRITEDATA, html);
  // 至此，该url代表的页面上的内容全都写入到html这个string类型的变量中了

  CURLcode success = curl_easy_perform(handle);
  if (success != CURLE_OK) {
    Log(ERROR) << "GetHtml error! code:" << success <<endl;
    return false;
  }
  return true;
}

//将所有要访问的url提取出来，存进一个vector中
bool GetUrlList(const std::string& html, std::vector<std::string>* url_list) {
  
  boost::regex reg("jyxxnr\\.asp\\?id=\\d*");//c++的转义、正则表达式的转义、所以有两个 '\'
  //    /stay_the_course/article/details

  
  boost::smatch result; 	//result是一个数组，数组的每一个元素是一对迭代器，指向查找到的字符串的开始和结束
  std::string::const_iterator beg = html.begin();
  std::string::const_iterator end = html.end();

  while (boost::regex_search(beg, end, result, reg)) {
    std::string url = std::string("http://job.xatu.edu.cn/");  //http://blog.csdn.net/stay_the_course
    std::string id = result[0];  
	
    url_list->push_back(url + id);	  // 将详情页的链接拼接完整然后插入到vector中

    beg = result[0].second;		//更新迭代器指向下一个位置
  }
  return true;
}

bool GetTitle(const std::string& html, std::string* title) {
	
  size_t strong_beg = html.find("<strong>");//找到<strong>这个标签，然后提取出标题
  size_t strong_end = html.find("</strong>");
  
  if (strong_beg == std::string::npos || strong_end == std::string::npos) {
    Log(ERROR) << "GetTitle not find strong! " << html << endl;
    return false;
  }
  size_t title_beg = strong_beg + strlen("<strong>");
  
  if (title_beg >= strong_end) {
    Log(ERROR) << "GetTitle title beg error: " << title_beg << endl;
    return false;
  }
  *title = html.substr(title_beg, strong_end - title_beg);
  return true;
}


//以每一个详情页标题为文件名保存这个详情页的内容
//path 指示路径， num 标识文件号， file_name标识要写入的文件，html标识这存放页面的字符串
bool Save(const std::string& path, int num, const std::string& file_name, const std::string& html) 
{
  std::string fullpath = path + boost::lexical_cast<std::string>(num) + "." + file_name;
  //文件名的格式为 
  std::ofstream file(fullpath.c_str());
  if (!file.is_open()) {
    Log(ERROR) << "Save open failed:" << fullpath << endl;
    return false;
  }
  file << html;//将html这个字符串中的所有内容写入file文件中
  file.close();
  return true;
}


//转换编码格式的函数
bool ConvertEncode(const std::string& in, std::string* out) {
  const size_t BufSize = 1024 * 100;
  char* input = const_cast<char*>(in.c_str());
  size_t inbytes_left = in.size();
  char outbuf[BufSize] = {0};
  char* output = outbuf;
  size_t outbytes_left = BufSize;

  iconv_t cd = iconv_open("utf-8//IGNORE", "gbk");//制定编码格式，第一个是from转换的格式，第二个参数是to的格式
  int ret = iconv(cd, &input, &inbytes_left, &output, &outbytes_left);
  if (ret < 0) {
    iconv_close(cd);
    return false;
  }
  *out = outbuf;
  iconv_close(cd);
  return true;
}

void Run() {
  // 1. 获取到就业信息网主页的html
  Log(INFO) << "Crawler Start!";
  std::string url = "http://job.xatu.edu.cn/jyxx.asp";		//这里传入主页的url
  std::string html;
  bool ret = GetHtml(url, &html);		//获取主页的html页面数据，当做字符串写入html这个变量中
  if (!ret) {
    Log(ERROR) << "GetHtml failed!" << endl;
    return;
  }
  Log(INFO) << "GetHtml Done!" << endl;
  
  
  // 2. 解析html, 拿到所有的详情页url的列表
  std::vector<std::string> url_list;
  ret = GetUrlList(html, &url_list);
  if (!ret) {
    Log(ERROR) << "GetUrlList failed" << endl;
    return;
  }
  Log(INFO) << "GetUrlList Done! url_list size:" << url_list.size() << endl;
  
  
  // 3. 遍历这个列表
  for (size_t i = 0; i < url_list.size(); ++i) {
    // 4. 分别获取到每个详情页的html
    const std::string& detail_url = url_list[i];		//做引用防止深拷贝，用以提高性能
    std::string detail_html;
    ret = GetHtml(detail_url, &detail_html);		//用详情页的url来获取每一个详情页的html数据，然后写入detail_html中
    if (!ret) {
      Log(ERROR) << "GetDetailHtml failed: " << detail_url << endl;
      continue;
    }
    // 4.5 转换编码格式
    std::string detail_html_utf8;
    ret = ConvertEncode(detail_html, &detail_html_utf8);
    if (!ret) {
      Log(ERROR) << "ConvertEncode failed" << endl;
      continue;
    }
    // 5. 解析出详情页的标题
    std::string title;
    ret = GetTitle(detail_html_utf8, &title);
    if (!ret) {
      Log(ERROR) << "GetTitle failed: " << detail_url << endl;
      continue;
    }
    // 6. 把详情页的html按照标题作为文件名, 存储在磁盘上.
    ret = Save("../data/", i + 1, title, detail_html_utf8);
    if (!ret) {
      Log(ERROR) << "Save failed: " << detail_url << endl;
      continue;
    }
    Log(INFO) << "Crawler: " << detail_url << " Done!" << endl;
  }
  Log(INFO) << "Crawler All Done!" << endl;
  return;
}

