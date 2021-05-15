#include <ngf/System/Scancode.h>
#include <EntityFactory.h>
#include "InputSystem.h"
#include "Keys.h"
#include "Component/Components.h"

namespace {
constexpr int SpaceshipShotDelay = 4;
constexpr int SpaceshipLoadShotFactor = 4;
constexpr int SpaceshipMaxLoadShot = 5;

void moveUp(entt::registry &registry, entt::entity e) {
  auto sc = registry.try_get<ShipComponent>(e);
  auto mc = registry.try_get<MotionComponent>(e);
  if (sc->direction == ShipDirection::Up) {
    if (sc->frameDelay) {
      --sc->frameDelay;
    } else {
      sc->direction = ShipDirection::Upper;
    }
  } else if (sc->direction == ShipDirection::None) {
    mc->velocity.y = -3.f;
    sc->direction = ShipDirection::Up;
    sc->frameDelay = 6;
  } else if (sc->direction >= ShipDirection::Down) {
    mc->velocity.y = 0.f;
    sc->direction = ShipDirection::None;
  }
}

void moveDown(entt::registry &registry, entt::entity e) {
  auto sc = registry.try_get<ShipComponent>(e);
  auto mc = registry.try_get<MotionComponent>(e);
  if (sc->direction == ShipDirection::Down) {
    if (sc->frameDelay) {
      --sc->frameDelay;
    } else {
      sc->direction = ShipDirection::MoreDown;
    }
  } else if (sc->direction == ShipDirection::None) {
    mc->velocity.y = 3.f;
    sc->direction = ShipDirection::Down;
    sc->frameDelay = 6;
  } else if (sc->direction <= ShipDirection::Up) {
    mc->velocity.y = 0.f;
    sc->direction = ShipDirection::None;
  }
}

void moveLeft(entt::registry &registry, entt::entity e) {
  auto mc = registry.try_get<MotionComponent>(e);
  mc->velocity.x = -3.f;
}

void moveRight(entt::registry &registry, entt::entity e) {
  auto mc = registry.try_get<MotionComponent>(e);
  mc->velocity.x = 3.f;
}

void still(entt::registry &registry, entt::entity e) {
  auto sc = registry.try_get<ShipComponent>(e);
  auto mc = registry.try_get<MotionComponent>(e);
  mc->velocity.y = 0.f;
  sc->direction = ShipDirection::None;
}

void stillRL(entt::registry &registry, entt::entity e) {
  auto mc = registry.try_get<MotionComponent>(e);
  mc->velocity.x = 0.f;
}

void shoot(entt::registry &registry, entt::entity e) {
  auto &sc = registry.get<ShipComponent>(e);
  if (sc.pulsedShot) {
    ++sc.power;
  } else {
    sc.pulsedShot = true;
    sc.power = 0;
  }
}

void noShoot(entt::registry &registry, entt::entity e) {
  auto &sc = registry.get<ShipComponent>(e);
  const auto &pc = registry.get<PositionComponent>(e);

  // shoot when player releases key
  if (sc.pulsedShot) {
    sc.pulsedShot = false;

    // do not allow to shoot too fast
    auto interval = sc.aliveTime - sc.lastShotTime;
    if (interval < sc.shotDelay) {
      if (interval < 0)
        sc.lastShotTime = 0;
      return;
    }
    sc.lastShotTime = sc.aliveTime;

    // put the new shot in the level
    int size = std::max(0, sc.power / SpaceshipShotDelay);
    size = std::min(SpaceshipMaxLoadShot, size / SpaceshipLoadShotFactor);
    EntityFactory::createSpaceshipShot(registry, pc.pos, size);
  }
  sc.power = 0;
}
} // namespace

namespace InputSystem {
void update(entt::registry &registry, const Keys &keys) {
  const auto view = registry.view<ShipComponent>();
  for (const entt::entity e : view) {
    auto &sc = view.get<ShipComponent>(e);
    if (sc.state != EntityState::Alive)
      continue;
    if (keys[ngf::Scancode::Up])
      moveUp(registry, e);
    if (keys[ngf::Scancode::Down])
      moveDown(registry, e);
    if (keys[ngf::Scancode::Left])
      moveLeft(registry, e);
    if (keys[ngf::Scancode::Right])
      moveRight(registry, e);
    if (!keys[ngf::Scancode::Up] && !keys[ngf::Scancode::Down])
      still(registry, e);
    if (!keys[ngf::Scancode::Left] && !keys[ngf::Scancode::Right])
      stillRL(registry, e);
    if (keys[ngf::Scancode::Space])
      shoot(registry, e);
    else
      noShoot(registry, e);
  }
}
}