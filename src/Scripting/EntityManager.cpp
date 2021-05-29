#include "EntityManager.h"

EntityManager::EntityManager(entt::registry &registry, sol::state &lua) :
    m_registry(registry), m_lua(lua) {}

Entity &EntityManager::createEntity() {
  auto e = m_registry.create();
  auto inserted = entities.emplace(e, std::make_unique<Entity>(m_registry, e));
  auto it = inserted.first; // iterator to created id/Entity pair
  auto &entity = *it->second; // created entity
  m_lua["createHandle"](entity);
  return entity;
}

void EntityManager::removeEntity(EntityId id) {
  m_lua["onEntityRemoved"](id);
  entities.erase(id);
  m_registry.destroy(id);
}

Entity *EntityManager::getEntity(EntityId id) const {
  auto it = entities.find(id);
  if (it == entities.end())
    return nullptr;
  return it->second.get();
}