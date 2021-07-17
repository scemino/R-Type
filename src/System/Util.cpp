#include "Util.h"
#include <fstream>

std::string readAllText(const fs::path& path) {
  std::ifstream input(path);

  std::string content;
  input.seekg(0, std::ios::end);
  content.reserve(input.tellg());
  input.seekg(0, std::ios::beg);

  content.assign((std::istreambuf_iterator<char>(input)),
                 std::istreambuf_iterator<char>());
  return content;
}
