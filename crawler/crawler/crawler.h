#pragma once
#include <vector>
#include <curl/curl.h>
#include <boost/regex.hpp>
#include "log.h"

bool GetHtml(const std::string& url, std::string* html);
bool GetUrlList(const std::string& html,
      std::vector<std::string>* url_list);
bool GetTitle(const std::string& html, std::string* title);
bool Save(const std::string& path, int num,
    const std::string& file_name, const std::string& html);
void Run();
