#include "Component/Components.h"
#include "CollisionSystem.h"
#include "Engine.h"
#include "Level.h"
#include <entt/entt.hpp>

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
    auto pEngine = registry.ctx<Engine *>();
    auto pEntity = pEngine->entityManager().getEntity(e);
    pEngine->eventManager().publish(pEntity,
                                    "hit",
                                    "collisionType",
                                    collisionValue.tile() ? "tile" : "screen",
                                    "pos",
                                    collisionValue.pos());
  }
}
}
