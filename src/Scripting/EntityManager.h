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
  [[nodiscard]] Entity *getEntity(EntityId id) const;
  void removeDeadEntities();

private:
  entt::registry &m_registry;
  sol::state &m_lua;
  std::unordered_map<EntityId, std::unique_ptr<Entity>> entities;
};
