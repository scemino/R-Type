#include "Component/Components.h"
#include "CollisionSystem.h"
#include "Engine.h"
#include "Level.h"
#include <entt/entt.hpp>

namespace CollisionSystem {

void collide(entt::registry &registry) {
  const auto level = registry.ctx<Level *>();
  const auto view = registry.view<CollideComponent, PositionComponent>();
  for (const entt::entity e : view) {
    // look for collisions with the level
    auto &cc = registry.get<CollideComponent>(e);
    auto &pc = registry.get<PositionComponent>(e);
    auto rect = ngf::irect::fromPositionSize(pc.pos, cc.size);
    auto collision = level->collideLevel(rect);
    const auto invincible = registry.try_get<InvincibleComponent>(e);

    if (invincible) {

      if (collision.has_value() && collision.value().tile()) {
        // if it is invincible and has collided with a tile, fix its position
        if (collision.value().left() && collision.value().right() &&
            collision.value().up() && collision.value().down()) {
        } else {
          if (collision.value().up() && !(collision.value().down()))
            pc.pos.y = static_cast<float>(pc.pos.y + collision.value().pos().y - rect.min.y);
          else if (collision.value().down() && !(collision.value().up()))
            pc.pos.y = static_cast<float>(pc.pos.y + collision.value().pos().y - (rect.min.y + rect.getHeight()));
          if (collision.value().left() && !(collision.value().right()))
            pc.pos.x = static_cast<float>(pc.pos.x + collision.value().pos().x - rect.min.x);
          else if (collision.value().right() && !(collision.value().left()))
            pc.pos.x = static_cast<float>(pc.pos.x + collision.value().pos().x - (rect.min.x + rect.getWidth()));
        }

        // ok, now it is possible that it goes out of the limits of the screen
        // if so, push the ship inside
        rect = ngf::irect::fromPositionSize(pc.pos, cc.size);
        collision = level->collideLevel(rect);
      }

      if (collision.has_value() && collision.value().screen()) {
        if (collision.value().up())
          pc.pos.y = static_cast<float>(pc.pos.y + collision.value().pos().y - rect.min.y);
        else if (collision.value().down())
          pc.pos.y = static_cast<float>(pc.pos.y + collision.value().pos().y - (rect.min.y + rect.getHeight()));
        if (collision.value().left())
          pc.pos.x = static_cast<float>(pc.pos.x + collision.value().pos().x - rect.min.x);
        else if (collision.value().right())
          pc.pos.x = static_cast<float>(pc.pos.x + collision.value().pos().x - (rect.min.x + rect.getWidth()));
      }
    } else {
      const auto isShip = registry.try_get<ShipComponent>(e);

      // goes off the screen?
      if (collision.has_value() && collision.value().screen()) {
        if (isShip) {
          // force the ship to be always on screen
          if (collision.value().up())
            pc.pos.y = static_cast<float>(pc.pos.y + collision.value().pos().y - rect.min.y);
          else if (collision.value().down())
            pc.pos.y = static_cast<float>(pc.pos.y + collision.value().pos().y - (rect.min.y + rect.getHeight()));
          if (collision.value().left())
            pc.pos.x = static_cast<float>(pc.pos.x + collision.value().pos().x - rect.min.x);
          else if (collision.value().right())
            pc.pos.x = static_cast<float>(pc.pos.x + collision.value().pos().x - (rect.min.x + rect.getWidth()));
          // maybe when pushing the ship, it collides with a tile
          rect = ngf::irect::fromPositionSize(pc.pos, cc.size);
          collision = level->collideLevel(rect);
        } else {
          registry.destroy(e);
        }
      }

      // hit a tile? die!
      if (collision && collision.value().tile()) {
        auto hc = registry.try_get<HealthComponent>(e);
        if (hc) {
          hc->health = 0;
        }
        auto shot = registry.try_get<ShotComponent>(e);
        if (shot) {
          registry.remove<CollideComponent>(e);
          registry.remove<MotionComponent>(e);
          shot->state = EntityState::Explode;
          shot->frameDelay = 6;
          shot->frameIndex = 0;
        }
      }
    }
  }
}
}