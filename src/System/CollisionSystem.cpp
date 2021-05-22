#include "Component/Components.h"
#include "CollisionSystem.h"
#include "Engine.h"
#include "Level.h"
#include <entt/entt.hpp>

namespace Systems::CollisionSystem {

void update(entt::registry &registry) {
  const auto **level = registry.try_ctx<const Level *>();
  if (!level)
    return;

  const auto view = registry.view<CollideComponent, PositionComponent>();
  for (const entt::entity e : view) {
    // look for collisions with the level
    const auto &[cc, pc] = registry.get<CollideComponent, PositionComponent>(e);
    auto rect = ngf::irect::fromPositionSize(pc.pos, cc.size);
    auto collision = (*level)->collideLevel(rect);

    if (!collision.has_value())
      continue;

    // hit a tile or screen?
    auto collisionValue = collision.value();
    if (collisionValue.tile()) {
      registry.emplace<HitTileComponent>(e, collisionValue);
    } else {
      registry.emplace<HitScreenComponent>(e, collisionValue);
    }
  }
}
}
