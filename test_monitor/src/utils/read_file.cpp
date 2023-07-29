#include "utils/read_file.h"
//传入一个字符串数组，该函数会将构造函数对应的文件里的一行内容的元素进行拆分，并依次传递到该数组中
namespace monitor {
bool ReadFile::ReadLine(std::vector<std::string>* args) {
  std::string line;
  std::getline(ifs_, line);
  if (ifs_.eof() || line.empty()) {
    return false;
  }

  std::istringstream line_ss(line);
  while (!line_ss.eof()) {
    std::string word;
    line_ss >> word;
    args->push_back(word);
  }
  return true;
}


}  // namespace monitor