#include "Components.h"
#include <Locator.h>
#include <AnimationLoader.h>

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
  auto animsInfo = loadAnimations(locator::engine::ref(), name);
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
