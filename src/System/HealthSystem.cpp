#include "Component/Components.h"
#include "HealthSystem.h"

namespace Systems::HitSystem {

void update(entt::registry &registry) {
  const auto view = registry.view<PositionComponent, HitTileComponent, InvincibleComponent, CollideComponent>();
  for (const entt::entity e : view) {
    auto &pc = registry.get<PositionComponent>(e);
    const auto &[hc, cc] = registry.get<HitTileComponent, CollideComponent>(e);
    // if it is invincible and has collided with a tile, fix its position
    if (hc.collision.left() && hc.collision.right() &&
        hc.collision.up() && hc.collision.down()) {
    } else {
      if (hc.collision.up() && !(hc.collision.down()))
        pc.pos.y = hc.collision.pos().y;
      else if (hc.collision.down() && !(hc.collision.up()))
        pc.pos.y = hc.collision.pos().y - cc.size.y;
      if (hc.collision.left() && !(hc.collision.right()))
        pc.pos.x = hc.collision.pos().x;
      else if (hc.collision.right() && !(hc.collision.left()))
        pc.pos.x = hc.collision.pos().x - cc.size.x;
    }
    registry.remove<HitTileComponent>(e);
  }

  const auto hcView = registry.view<CollideComponent, HitTileComponent, HealthComponent>();
  for (const entt::entity e : hcView) {
    auto &hc = hcView.get<HealthComponent>(e);
    hc.health = 0;
    registry.emplace<ExplodeComponent>(e);
    registry.remove<CollideComponent>(e);
    registry.remove<HitTileComponent>(e);
  }

  const auto htOthersView = registry.view<HitTileComponent>();
  registry.destroy(htOthersView.begin(), htOthersView.end());

  const auto hsView = registry.view<InputStateComponent, HitScreenComponent, PositionComponent, CollideComponent>();
  for (const entt::entity e : hsView) {
    const auto &[hc, cc] = hsView.get<HitScreenComponent, CollideComponent>(e);
    auto &pc = hsView.get<PositionComponent>(e);

    if (hc.collision.up())
      pc.pos.y = hc.collision.pos().y;
    else if (hc.collision.down())
      pc.pos.y = hc.collision.pos().y - cc.size.y;
    if (hc.collision.left())
      pc.pos.x = hc.collision.pos().x;
    else if (hc.collision.right())
      pc.pos.x = hc.collision.pos().x - cc.size.x;

    registry.remove<HitScreenComponent>(e);
  }

  const auto hsOthersView = registry.view<HitScreenComponent>();
  registry.destroy(hsOthersView.begin(), hsOthersView.end());
}
}
