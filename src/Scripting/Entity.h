#pragma once

#include <string>
#include <entt/entt.hpp>
#include <sol/sol.hpp>
#include <glm/vec2.hpp>

struct Entity {
  explicit Entity(entt::registry &registry, entt::entity e);

  [[nodiscard]] entt::entity getId() const { return m_entity; }

  void emplace(sol::variadic_args va);

  void setPos(const glm::ivec2 &pos);
  [[nodiscard]] glm::ivec2 getPos() const;

  void setAnim(const std::string& anim, int loop = 1);
  [[nodiscard]] std::string getAnim() const;

  entt::registry &registry() { return m_registry; }

private:
  entt::registry &m_registry;
  entt::entity m_entity;
};
