#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;
std::string readAllText(const fs::path& path);
