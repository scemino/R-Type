#pragma once

#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include <unordered_map>
#include <memory>
#include <ECS/Entity.h>

using EntityId = entt::entity;

class EntityManager {
public:
  EntityManager(entt::registry &registry, sol::state &lua);

  Entity &createEntity();
  void destroyEntity(EntityId id);
  void removeDeadEntities();

  /// Indicates whether or not an entity exists.
  /// \param id Name of the entity to find.
  /// \return true if the entity exists.
  [[nodiscard]] bool hasEntity(const std::string& name) const;

  /// Gets entity by its id.
  /// \param id Id of the entity to find.
  /// \return The entity found.
  [[nodiscard]] Entity &getEntity(EntityId id) const;

  /// Gets entity by its name.
  /// \param name Name of the entity to find.
  /// \return The entity found.
  [[nodiscard]] Entity &getEntity(const std::string& name) const;

private:
  entt::registry &m_registry;
  sol::state &m_lua;
  std::unordered_map<EntityId, std::unique_ptr<Entity>> entities;
};
