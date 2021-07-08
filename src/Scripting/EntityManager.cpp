#include <cassert>
#include <System/Log.h>
#include <ECS/Component/Components.h>
#include "EntityManager.h"

EntityManager::EntityManager(entt::registry &registry, sol::state &lua) :
    m_registry(registry), m_lua(lua) {}

Entity &EntityManager::createEntity() {
  auto e = m_registry.create();
  RTYPE_LOG_DEBUG("Create entity {}", e);
  auto inserted = entities.emplace(e, std::make_unique<Entity>(e));
  auto it = inserted.first; // iterator to created id/Entity pair
  auto &entity = *it->second; // created entity
  m_lua["createHandle"](entity);
  return entity;
}

void EntityManager::destroyEntity(EntityId id) {
  RTYPE_LOG_DEBUG("Mark entity {} as dead", id);
  getEntity(id).die();
}

void EntityManager::removeDeadEntities() {
  for (auto it = entities.begin(); it != entities.end();) {
    if (it->second->isDead()) {
      auto hc = it->second->tryComponent<HierarchyComponent>();
      if (hc.has_value()) {
        // remove entity from parent's list
        auto parent = hc.value().get().getParent();
        if (parent.has_value()) {
          parent.value().get().component<HierarchyComponent>().removeChild(*it->second);
        }

        // TODO: detach all children
      }
      RTYPE_LOG_DEBUG("Destroy entity {}", it->second->getId());
      m_lua["onEntityRemoved"](it->second->getId());
      m_registry.destroy(it->second->getId());
      it = entities.erase(it);
    } else {
      ++it;
    }
  }
}

Entity &EntityManager::getEntity(EntityId id) const {
  auto it = entities.find(id);
  if (it == entities.end()) {
    RTYPE_LOG_ERROR("Entity {} has not been found", id);
    assert(false);
  }
  return *it->second;
}

Entity &EntityManager::getEntity(const std::string &name) const {
  auto it = std::find_if(entities.begin(), entities.end(), [&name](const auto &pair) {
    Entity &e = *pair.second;
    return e.component<NameComponent>().name == name;
  });
  if (it == entities.end()) {
    RTYPE_LOG_ERROR("Entity {} has not been found", name);
    assert(false);
  }
  return *it->second;
}

bool EntityManager::hasEntity(const std::string &name) const {
  auto it = std::find_if(entities.begin(), entities.end(), [&name](const auto &pair) {
    Entity &e = *pair.second;
    return e.component<NameComponent>().name == name;
  });
  return it != entities.end();
}
