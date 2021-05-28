#include "Entity.h"
#include "Component/Components.h"
#include "Engine.h"
#include <AnimationLoader.h>

Entity::Entity(entt::registry &registry) : m_registry(registry) {
  m_entity = m_registry.create();
}

void Entity::emplace(sol::variadic_args va) {
  if (va.size() == 0)
    return;
  const auto name = va.get<std::string_view>(0);
  if (name == "Position") {
    m_registry.emplace<PositionComponent>(m_entity);
    return;
  }
  if (name == "Name") {
    const auto n = va.get<std::string>(1);
    m_registry.emplace<NameComponent>(m_entity, n);
    return;
  }
  if (name == "Graphics") {
    auto &gc = m_registry.emplace<GraphicComponent>(m_entity);
    gc.texture = m_registry.ctx<Engine *>()->loadTexture(va[1]);
    auto r = va[2].as<std::array<int, 4>>();
    const auto
        rect = ngf::irect::fromPositionSize({r[0], r[1]}, {r[2], r[3]});
    gc.frame = rect;
  }
  if (name == "Animation") {
    auto &ac = m_registry.emplace<AnimationComponent>(m_entity);
    auto pEngine = m_registry.ctx<Engine *>();
    auto animsInfo = loadAnimations(*pEngine, va.get<std::string>(1));
    ac.animations = animsInfo.animations;
    ac.current = animsInfo.initialAnim;
    return;
  }
}

void Entity::setPos(const glm::ivec2 &pos) {
  auto pc = m_registry.try_get<PositionComponent>(m_entity);
  if (!pc)
    return;
  pc->pos = pos;
}

glm::ivec2 Entity::getPos() const {
  const auto pc = m_registry.try_get<PositionComponent>(m_entity);
  if (!pc)
    return {};
  return pc->pos;
}