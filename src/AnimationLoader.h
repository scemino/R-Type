#include <filesystem>
#include <unordered_map>
#include "Component/Components.h"
#include "Engine.h"

std::unordered_map<std::string, Animation> loadAnimations(Engine &engine, const std::filesystem::path& path);
