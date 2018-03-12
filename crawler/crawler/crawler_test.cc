#include <gtest/gtest.h>
#include "crawler.h"

TEST(Crawler, Log) {
  Log(INFO) << "hello" << endl;
}

#if 0
TEST(Crawler, GetHtml) {
  std::string url = "http://job.xatu.edu.cn/jyxx.asp";
  std::string html;
  ASSERT_TRUE(GetHtml(url, &html));
  ASSERT_GT(html.size(), 1000);
  Log(DEBUG) << html << endl;
}
#endif

TEST(Crawler, GetUrlList) {
  std::string html = "<td width=\"70%\" height=\"28\" align=\"left\" ><a href=\"jyxxnr.asp?id=3983&mnid=7726&classname=%B5%A5%CE%BB%D0%E8%C7%F3%D0%C5%CF%A2\" target=\"_blank\">昆明荣者光电科技发展有限公司（招聘启事）</a>&nbsp;<font color=\"#FF0000\"></font></td>";
  std::vector<std::string> url_list;
  ASSERT_TRUE(GetUrlList(html, &url_list));
  ASSERT_EQ(url_list.size(), 1);
  ASSERT_EQ(url_list[0], "http://job.xatu.edu.cn/jyxxnr.asp?id=3983");
}

#if 0
TEST(Crawler, GetDetailHtml) {
  std::string url = "http://job.xatu.edu.cn/jyxxnr.asp?id=3978";
  std::string html;
  ASSERT_TRUE(GetHtml(url, &html));
  Log(DEBUG) << html << endl;
}
#endif

TEST(Crawler, GetTitle) {
  std::string html = "<td height=\"50\" align=\"center\" class=\"cu sb\"><strong>甘肃中海投资管理有限公司2017招聘公告</strong></td>";
  std::string title;
  ASSERT_TRUE(GetTitle(html, &title));
  ASSERT_EQ(title, "甘肃中海投资管理有限公司2017招聘公告");
}

TEST(Crawler, Save) {
  std::string path = "../test_data/";
  std::string file_name = "文件.html";
  std::string html = "hello";
  int num = 1;
  // ../test_data/1.文件.html
  ASSERT_TRUE(Save(path, num, file_name, html));
}
