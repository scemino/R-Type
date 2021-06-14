#include "Components.h"
#include <Graphics/AnimationLoader.h>

PositionComponent::PositionComponent(const sol::table &t) {
  const auto p = t["pos"].get<glm::vec2>();
  pos = p;
}

void PositionComponent::setPosition(const glm::vec2 &p) {
  pos = p;
  auto &r = locator::engine::ref().registry();
  auto e = entt::to_entity(r, *this);
  auto hc = r.try_get<HierarchyComponent>(e);
  if(!hc) return;
  hc->updateParentOffset();
  hc->updateChildrenPosition();
}

glm::vec2 PositionComponent::getPosition() const { return pos; }

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

HierarchyComponent::HierarchyComponent(const sol::table &t) {
}