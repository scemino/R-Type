#pragma once

#include <string>
#include <entt/entt.hpp>
#include <System/Locator.h>

struct Entity {
  explicit Entity(entt::entity e);

  [[nodiscard]] entt::entity getId() const { return m_entity; }

  void die();
  bool isDead() const;

  template<typename T>
  const T& component() const;

  template<typename T>
  T& component();

private:
  entt::entity m_entity;
  bool m_isDead{false};
};

template<typename T>
const T& Entity::component() const
{
  const T* component = locator::engine::ref().registry().try_get<T>(m_entity);
  if (!component) {
    throw std::runtime_error("Component not found!");
  }
  return *component;
}

template<typename T>
T& Entity::component()
{
  auto component = locator::engine::ref().registry().try_get<T>(m_entity);
  if (!component) {
    throw std::runtime_error("Component not found!");
  }
  return *component;
}