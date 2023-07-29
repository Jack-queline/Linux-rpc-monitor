#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace monitor {
class ReadFile {
 public:
 //构造函数，接受一个文件名，并利用ifs打开
 //其中explicit关键字用于防止类构造函数的隐式自动转换。
  explicit ReadFile(const std::string& name) : ifs_(name) {}
  ~ReadFile() { ifs_.close(); }
  //读取一行的数据，对数据中的内容进行拆分，并存储在arg指向的数组中
  bool ReadLine(std::vector<std::string>* args);
  //传递一个文件名，和文件的行数，将每一行拆分存储在字符串数组中
  static std::vector<std::string> GetStatsLines(const std::string& stat_file,
                                                const int line_count) {
    std::vector<std::string> stats_lines;
    std::ifstream buffer(stat_file);
    for (int line_num = 0; line_num < line_count; ++line_num) {
      std::string line;
      std::getline(buffer, line);
      if (line.empty()) {
        break;
      }
      stats_lines.push_back(line);
    }
    return stats_lines;
  }

 private:
  std::ifstream ifs_;
};
}  // namespace monitor
