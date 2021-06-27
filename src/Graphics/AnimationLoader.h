#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <ECS/Component/Components.h>
#include <Graphics/Animation.h>

struct AnimationsInfo {
  std::unordered_map<std::string, Animation> animations;
  std::string initialAnim;

  AnimationsInfo(std::unordered_map<std::string, Animation> animations, std::string initialAnim);
};

struct AnimationLoader final {
  [[nodiscard]] std::shared_ptr<AnimationsInfo> load(const std::filesystem::path &path) const;
};
