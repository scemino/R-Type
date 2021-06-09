#include "Entity.h"
#include "Component/Components.h"
#include "Engine.h"
#include <AnimationLoader.h>

Entity::Entity(entt::registry &registry, entt::entity e) : m_registry(registry), m_entity(e) {
}

void Entity::emplace(sol::variadic_args va) {
  if (va.size() == 0)
    return;
  const auto name = va.get<std::string_view>(0);
  if (name == "Position") {
    m_registry.emplace<PositionComponent>(m_entity);
    return;
  }
  if (name == "Motion") {
    if (va.size() == 2) {
      const auto v = va[1].as<std::array<int, 2>>();
      m_registry.emplace<MotionComponent>(m_entity, glm::vec2({v[0], v[1]}));
    } else {
      m_registry.emplace<MotionComponent>(m_entity);
    }
    return;
  }
  if (name == "Name") {
    const auto n = va.get<std::string>(1);
    m_registry.emplace<NameComponent>(m_entity, n);
    return;
  }
  if (name == "Graphics") {
    if (va.size() == 3) {
      auto texture = m_registry.ctx<Engine *>()->loadTexture(va[1]);
      auto r = va[2].as<std::array<int, 4>>();
      const auto
          frame = ngf::irect::fromPositionSize({r[0], r[1]}, {r[2], r[3]});
      m_registry.emplace<GraphicComponent>(m_entity, texture, frame);
    } else {
      m_registry.emplace<GraphicComponent>(m_entity);
    }
    return;
  }
  if (name == "Animation") {
    auto &ac = m_registry.emplace<AnimationComponent>(m_entity);
    auto pEngine = m_registry.ctx<Engine *>();
    auto animsInfo = loadAnimations(*pEngine, va.get<std::string>(1));
    ac.animations = animsInfo.animations;
    ac.setAnim(animsInfo.initialAnim, -1);
    return;
  }
  if (name == "Collide") {
    auto size = va[1].as<std::array<int, 2>>();
    m_registry.emplace<CollideComponent>(m_entity, glm::vec2({size[0], size[1]}));
    return;
  }
}
