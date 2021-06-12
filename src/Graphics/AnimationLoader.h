#include <filesystem>
#include <string>
#include <unordered_map>
#include <ECS/Component/Components.h>
#include <Graphics/Animation.h>

struct AnimationsInfo {
  std::unordered_map<std::string, Animation> animations;
  std::string initialAnim;
};

AnimationsInfo loadAnimations(const std::filesystem::path &path);
