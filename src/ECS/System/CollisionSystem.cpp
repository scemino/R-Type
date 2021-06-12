#include "CollisionSystem.h"
#include <entt/entt.hpp>
#include <ECS/Component/Components.h>
#include <Level.h>
#include <System/Locator.h>
#include <Scripting/EntityManager.h>
#include <Scripting/EventManager.h>

namespace Systems::CollisionSystem {

void update(entt::registry &registry) {
  const auto level = registry.ctx<Level *>();
  if (!level)
    return;

  const auto view = registry.view<CollideComponent, PositionComponent>();
  for (const entt::entity e : view) {
    // look for collisions with the level
    const auto &[cc, pc] = registry.get<CollideComponent, PositionComponent>(e);
    auto rect = ngf::irect::fromPositionSize(pc.pos, cc.size);
    auto collision = level->collideLevel(rect);

    if (!collision.has_value())
      continue;

    // notify entity collision with tile or screen
    auto collisionValue = collision.value();
    auto& engine = locator::engine::ref();
    auto pEntity = engine.entityManager().getEntity(e);
    engine.eventManager().publish(pEntity,
                                    "hit",
                                    "collisionType",
                                    collisionValue.tile() ? "tile" : "screen",
                                    "pos",
                                    collisionValue.pos());
  }
}
}