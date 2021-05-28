#include <filesystem>
#include <unordered_map>
#include "Component/Components.h"
#include "Engine.h"

struct AnimationsInfo {
  std::unordered_map<std::string, Animation> animations;
  std::string initialAnim;
};

AnimationsInfo loadAnimations(Engine &engine, const std::filesystem::path &path);
