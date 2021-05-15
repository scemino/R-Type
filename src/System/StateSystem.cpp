#include "StateSystem.h"
#include "Component/Components.h"

namespace {
constexpr int beamLoad[8][4] = {{2, 51, 31, 32}, {35, 52, 28, 28}, {68, 54, 29, 28},
                                {103, 55, 25, 25}, {135, 56, 29, 22}, {168, 58, 26, 18},
                                {200, 59, 23, 15}, {233, 60, 20, 12}};

constexpr int flash[2][4] = {{215, 85, 14, 12}, {233, 87, 11, 9}};

constexpr int flashMid = 91;

void updateSpaceship(entt::registry &registry) {
  const auto view = registry.view<ShipComponent, HealthComponent>();
  for (const entt::entity e : view) {
    auto &sc = view.get<ShipComponent>(e);
    auto &hc = view.get<HealthComponent>(e);
    if (sc.state == EntityState::Alive) {
      if (hc.health > 0) {
        sc.aliveTime++;
      } else {
        sc.aliveTime = 0;
        sc.lastShotTime = 0;
        sc.state = EntityState::Explode;
        sc.frameDelay = 6;
        sc.frameIndex = 0;
        registry.remove<CollideComponent>(e);
        registry.remove<MotionComponent>(e);
      }
    } else if (sc.state == EntityState::Explode) {
      --sc.frameDelay;
      if (sc.frameDelay == 0) {
        if (sc.frameIndex < 8) {
          sc.frameDelay = 6;
          ++sc.frameIndex;
        } else {
          hc.lives--;
          if (hc.lives) {
            hc.health = 100;
            sc.state = EntityState::Alive;
            registry.emplace_or_replace<PositionComponent>(e, glm::vec2(32, 60));
            registry.emplace<MotionComponent>(e);
            registry.emplace<CollideComponent>(e, glm::vec2(32, 14));
            registry.emplace<InvincibleComponent>(e, 240);
          }
        }
      }
    }
  }
}

void updateShot(entt::registry &registry) {
  const auto view = registry.view<ShotComponent>();
  for (const entt::entity e : view) {
    auto &sc = view.get<ShotComponent>(e);
    if (sc.state == EntityState::Alive) {
      if (sc.frameDelay)
        --sc.frameDelay;
      else {
        sc.frameIndex = !sc.frameIndex;
        sc.frameDelay = 6;
      }
    } else {
      if (sc.frameDelay)
        --sc.frameDelay;
      else {
        if (sc.frameIndex) {
           registry.destroy(e);
        } else {
          sc.frameIndex = !sc.frameIndex;
          sc.frameDelay = 6;
        }
      }
    }
  }
}

}

namespace StateSystem {

void update(entt::registry &registry) {
  updateSpaceship(registry);
  updateShot(registry);
}
}