#include "Components.h"
#include <Graphics/AnimationLoader.h>

PositionComponent::PositionComponent(const sol::table &t) {
  const auto p = t["pos"].get<glm::vec2>();
  pos = p;
}

MotionComponent::MotionComponent(const sol::table &t) {
  const auto v = t["velocity"].get<glm::vec2>();
  velocity = v;
}

CollideComponent::CollideComponent(const sol::table &t) {
  const auto s = t["size"].get<glm::vec2>();
  size = s;
}

NameComponent::NameComponent(const sol::table &t) {
  const auto n = t["name"].get<std::string>();
  name = n;
}

AnimationComponent::AnimationComponent(const sol::table &t) {
  const auto name = t["name"].get<std::string>();
  auto animsInfo = loadAnimations(name);
  animations = animsInfo.animations;
  setAnim(animsInfo.initialAnim, -1);
}

GraphicComponent::GraphicComponent(const sol::table &t) {
}

void AnimationComponent::setAnim(const std::string &anim, int loop) {
  this->current = anim;
  this->frameIndex = 0;
  this->loop = loop;
  this->playing = true;
}
