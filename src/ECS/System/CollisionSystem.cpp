#include "CollisionSystem.h"
#include <entt/entt.hpp>
#include <ECS/Component/Components.h>
#include <Level.h>
#include <System/Locator.h>
#include <Scripting/EventManager.h>

namespace {
ngf::frect getHitBox(const ngf::frect &rect, const glm::vec2 &pos) {
  ngf::frect r = rect;
  r.min += pos;
  r.max += pos;
  return r;
}

ngf::frect getHitBox(const Entity &entity) {
  const auto rect = entity.component<CollideComponent>().hitbox;
  const auto pos = entity.component<PositionComponent>().getPosition();
  return getHitBox(rect, pos);
}

/// Check collision between 2 entities, if there is a collision, sends an event.
/// \param id1 id of entity 1
/// \param id2 id of entity 2
void checkCollision(const entt::entity id1, const entt::entity id2) {
  if (id1 == id2)
    return;

  auto &engine = locator::engine::ref();
  const auto &etm = engine.entityManager();
  const auto& handles = engine.lua().get<sol::table>("Handles");
  const auto &e1 = etm.getEntity(id1);
  const auto &e2 = etm.getEntity(id2);
  if (!getHitBox(e1).intersects(getHitBox(e2)))
    return;

  auto &em = engine.eventManager();
  em.publish(handles[id1],
             "hit",
             "collisionType",
             "entities",
             "entity",
             handles[id2]);
  em.publish(handles[id2],
             "hit",
             "collisionType",
             "entities",
             "entity",
             handles[id1]);
}
}

namespace Systems::CollisionSystem {

void update(entt::registry &registry) {
  const auto level = registry.ctx<Level *>();
  if (!level)
    return;

  auto &engine = locator::engine::ref();
  const auto &etm = engine.entityManager();
  auto &em = engine.eventManager();
  const auto& handles = locator::engine::ref().lua().get<sol::table>("Handles");

  const auto view = registry.view<CollideComponent, PositionComponent>();
  for (const entt::entity e : view) {
    // look for collisions with the level
    const auto &[cc, pc] = registry.get<CollideComponent, PositionComponent>(e);
    auto rect = getHitBox(cc.hitbox, pc.getPosition());
    auto collision = level->collideLevel(ngf::irect::fromPositionSize(rect.getPosition(), rect.getSize()));

    if (!collision.has_value())
      continue;

    // notify entity collision with tile or screen
    const auto collisionValue = collision.value();
    const auto directionTable = engine.lua().create_table_with(
        "up", collisionValue.up(),
        "down", collisionValue.down(),
        "left", collisionValue.left(),
        "right", collisionValue.right());
    em.publish(handles[e],
               "hit",
               "collisionType",
               collisionValue.tile() ? "tile" : "screen",
               "dir",
               directionTable,
               "pos",
               glm::vec2(collisionValue.pos()));
  }

  auto itEnd = view.end();
  for (auto it1 = view.begin(); it1 != itEnd; ++it1) {
    for (auto it2 = it1; it2 != itEnd; ++it2) {
      checkCollision(*it1, *it2);
    }
  }
}
}
