#pragma once

#include <entt/entt.hpp>
#include <sol/sol.hpp>
#include <glm/vec2.hpp>

struct Entity {
  explicit Entity(entt::registry &registry);

  void emplace(sol::variadic_args va);

  void setPos(const glm::ivec2& pos);
  [[nodiscard]] glm::ivec2 getPos() const;

private:
  entt::registry &m_registry;
  entt::entity m_entity;
};
