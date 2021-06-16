#pragma once

#include <optional>
#include <string>
#include <entt/entt.hpp>
#include <System/Locator.h>

struct Entity {
  explicit Entity(entt::entity e);

  [[nodiscard]] entt::entity getId() const { return m_entity; }

  void die();
  [[nodiscard]] bool isDead() const;

  template<typename T>
  const T& component() const;

  template<typename T>
  std::optional<std::reference_wrapper<T>> tryComponent() const;

  template<typename T>
  T& component();

private:
  entt::entity m_entity;
  bool m_isDead{false};
};

template<typename T>
std::optional<std::reference_wrapper<T>> Entity::tryComponent() const
{
  T* component = locator::engine::ref().registry().try_get<T>(m_entity);
  if(!component) return std::nullopt;
  return std::ref(*component);
}

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