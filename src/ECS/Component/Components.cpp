#include "Components.h"
#include <Graphics/AnimationLoader.h>

PositionComponent::PositionComponent(const sol::table &t) {
  const auto p = t["pos"].get<glm::vec2>();
  pos = p;
}

void PositionComponent::setPosition(const glm::vec2 &p) {
  auto offset = p - pos;
  pos = p;
  auto &r = locator::engine::ref().registry();
  auto e = entt::to_entity(r, *this);
  auto hc = r.try_get<HierarchyComponent>(e);
  if (!hc)
    return;
  hc->updateParentOffset();
  hc->updateChildrenPosition(offset);
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

entt::entity HierarchyComponent::getEntity() const {
  auto &r = locator::engine::ref().registry();
  return entt::to_entity(r, *this);
}

void HierarchyComponent::addChild(Entity &entityChild) {
  const auto offset = getOffset(entityChild.getId());
  m_children.emplace_back(offset, entityChild.getId());
  entityChild.component<HierarchyComponent>().m_parentId = getEntity();
}

void HierarchyComponent::removeChild(const Entity &entityChild) {
  const auto id = entityChild.getId();
  auto it = std::find_if(m_children.begin(), m_children.end(), [id](const auto &child) {
    return child.entityId == id;
  });
  if (it == m_children.end())
    return;
  it->entityId = entt::null;
  m_children.erase(it);
}

void HierarchyComponent::updateParentOffset() {
  const auto &em = locator::engine::ref().entityManager();
  if (hasParent()) {
    auto parent = em.getEntity(m_parentId);
    parent.component<HierarchyComponent>().updateOffset(getEntity());
  }
}

void HierarchyComponent::updateChildrenPosition(const glm::vec2 &offset) {
  const auto &r = locator::engine::ref().registry();
  const auto parentPos = r.get<PositionComponent>(getEntity()).getPosition();

  const auto &em = locator::engine::ref().entityManager();
  for (const auto &child : m_children) {
    auto &childPc = em.getEntity(child.entityId).component<PositionComponent>();
    const auto childPos = childPc.getPosition() + offset;
    childPc.setPosition(childPos);
  }
}

void HierarchyComponent::updateOffset(entt::entity e) {
  auto it = std::find_if(m_children.begin(), m_children.end(), [&e](const auto &child) {
    return child.entityId == e;
  });
  if (it == m_children.end())
    return;
  it->offset = getOffset(e);
}

glm::vec2 HierarchyComponent::getOffset(entt::entity entityChild) const {
  auto &r = locator::engine::ref().registry();
  const auto offset = r.get<PositionComponent>(entityChild).getPosition() -
      r.get<PositionComponent>(getEntity()).getPosition();
  return offset;
}

[[nodiscard]] std::optional<std::reference_wrapper<Entity>> HierarchyComponent::getParent() {
  if (m_parentId == entt::null)
    return std::nullopt;
  const auto &em = locator::engine::ref().entityManager();
  return std::make_optional(std::ref(em.getEntity(m_parentId)));
}