#include <filesystem>
#include <unordered_map>
#include "Component/Components.h"

std::unordered_map<std::string, Animation> loadAnimations(const std::filesystem::path& path);
