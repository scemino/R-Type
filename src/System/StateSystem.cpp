#include "StateSystem.h"
#include "Component/Components.h"

namespace {
constexpr int beamLoad[8][4] = {{2, 51, 31, 32}, {35, 52, 28, 28}, {68, 54, 29, 28},
                                {103, 55, 25, 25}, {135, 56, 29, 22}, {168, 58, 26, 18},
                                {200, 59, 23, 15}, {233, 60, 20, 12}};

constexpr int flash[2][4] = {{215, 85, 14, 12}, {233, 87, 11, 9}};

constexpr int flashMid = 91;
constexpr int disMid = 66;

void updateSpaceship(entt::registry &registry) {
  const auto view = registry.view<ShipComponent, HealthComponent>();
  for (const entt::entity e : view) {
    auto &sc = view.get<ShipComponent>(e);
    auto &hc = view.get<HealthComponent>(e);
    if (sc.state == SpaceshipState::Alive) {
      if (hc.health > 0) {
        sc.aliveTime++;
      } else {
        sc.aliveTime = 0;
        sc.state = SpaceshipState::Explode;
        sc.delay = 8 * 6;
        registry.remove<CollideComponent>(e);
        registry.remove<MotionComponent>(e);
      }
    } else if (sc.state == SpaceshipState::Explode) {
      sc.delay--;
      if (sc.delay == 0) {
        hc.lives--;
        if (hc.lives) {
          hc.health = 100;
          sc.state = SpaceshipState::Alive;
          registry.emplace_or_replace<PositionComponent>(e, glm::vec2(32, 60));
          registry.emplace<MotionComponent>(e);
          registry.emplace<CollideComponent>(e, glm::vec2(32, 14));
          registry.emplace<InvincibleComponent>(e, 240);
        }
      }
    }
  }
}

void updateShot(entt::registry &registry) {
  const auto view = registry.view<ShotComponent>();
  for (const entt::entity e : view) {
    auto &sc = view.get<ShotComponent>(e);
    if (sc.state == SpaceshipState::Alive) {
      if (sc.delay)
        --sc.delay;
      else {
        sc.seq = !sc.seq;
        sc.delay = 6;
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