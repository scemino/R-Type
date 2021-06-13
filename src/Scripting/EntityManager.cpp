#include <System/Log.h>
#include "EntityManager.h"

EntityManager::EntityManager(entt::registry &registry, sol::state &lua) :
    m_registry(registry), m_lua(lua) {}

Entity &EntityManager::createEntity() {
  auto e = m_registry.create();
  RTYPE_LOG_INFO("Create entity {}", e);
  auto inserted = entities.emplace(e, std::make_unique<Entity>(e));
  auto it = inserted.first; // iterator to created id/Entity pair
  auto &entity = *it->second; // created entity
  m_lua["createHandle"](entity);
  return entity;
}

void EntityManager::destroyEntity(EntityId id) {
  RTYPE_LOG_INFO("Destroy entity {}", id);
  m_lua["onEntityRemoved"](id);
  auto pEntity = getEntity(id);
  if (pEntity)
    pEntity->die();
}

void EntityManager::removeDeadEntities() {
  for (auto it = entities.begin(); it != entities.end();) {
    if (it->second->isDead()) {
      m_registry.destroy(it->second->getId());
      it = entities.erase(it);
    } else {
      ++it;
    }
  }
}

Entity *EntityManager::getEntity(EntityId id) const {
  auto it = entities.find(id);
  if (it == entities.end())
    return nullptr;
  return it->second.get();
}