#pragma once

#include <iostream>
#include <string>
#include <ctime>

using std::endl;

enum LogLevel {
  DEBUG,
  INFO,
  ERROR
};

static inline std::ostream& LogInner(LogLevel level, const std::string& file, int line) {
  std::string prefix;
  if (level == DEBUG) {
    prefix = "[D";
  } else if (level == INFO) {
    prefix = "[I";
  } else if (level == ERROR) {
    prefix = "[E";
  }
  return std::cout << prefix << time(NULL) << " " << file << ":" << line << "] ";
}

#define Log(level) LogInner(level, __FILE__, __LINE__)
