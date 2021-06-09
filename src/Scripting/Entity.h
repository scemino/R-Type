#pragma once

#include <string>
#include <entt/entt.hpp>
#include <sol/sol.hpp>
#include <glm/vec2.hpp>

struct Entity {
  explicit Entity(entt::registry &registry, entt::entity e);

  [[nodiscard]] entt::entity getId() const { return m_entity; }

  void emplace(sol::variadic_args va);

  template<typename T>
  const T& component() const;

  template<typename T>
  T& component();

  entt::registry &registry() { return m_registry; }

private:
  entt::registry &m_registry;
  entt::entity m_entity;
};

template<typename T>
const T& Entity::component() const
{
  const T* component = m_registry.try_get<T>(m_entity);
  if (!component) {
    throw std::runtime_error("Component not found!");
  }
  return *component;
}

template<typename T>
T& Entity::component()
{
  auto component = m_registry.try_get<T>(m_entity);
  if (!component) {
    throw std::runtime_error("Component not found!");
  }
  return *component;
}