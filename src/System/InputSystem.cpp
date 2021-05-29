#include <ngf/System/Scancode.h>
#include <EntityFactory.h>
#include "InputSystem.h"
#include "Keys.h"
#include "Component/Components.h"

namespace {
constexpr int SpaceshipLoadShotFactor = 4;
constexpr int SpaceshipShotDelay = 4;
constexpr int SpaceshipMaxLoadShot = 5;

void moveUp(AnimationComponent &ac, InputStateComponent &ic, MotionComponent &mc) {
  if (!ic.up) {
    ic.up = true;
    mc.velocity.y = -3.f;
    ic.delay = 0;
    ac.current = "up";
    ac.delay = 0;
  } else if (ic.delay < 6) {
    ++ic.delay;
  } else {
    ac.current = "upper";
    ac.delay = 0;
  }
}

void moveDown(AnimationComponent &ac, InputStateComponent &ic, MotionComponent &mc) {
  if (!ic.down) {
    ic.down = true;
    mc.velocity.y = 3.f;
    ac.current = "down";
    ac.delay = 0;
  } else if (ic.delay < 6) {
    ++ic.delay;
  } else {
    ac.current = "moreDown";
    ac.delay = 0;
  }
}

void moveLeft(MotionComponent &mc) {
  mc.velocity.x = -3.f;
}

void moveRight(MotionComponent &mc) {
  mc.velocity.x = 3.f;
}

void still(AnimationComponent &ac, InputStateComponent &ic, MotionComponent &mc) {
  mc.velocity.y = 0.f;
  ic.up = false;
  ic.down = false;
  ac.current = "idle";
  ac.delay = 0;
}

void stillRL(MotionComponent &mc) {
  mc.velocity.x = 0.f;
}

void shoot(ShipComponent &sc) {
  if (sc.pulsedShot) {
    ++sc.power;
  } else {
    sc.pulsedShot = true;
    sc.power = 0;
  }
}

void noShoot(entt::registry &registry, ShipComponent &sc, const PositionComponent &pc) {
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
    // TODO:
//    EntityFactory::createSpaceshipShot(registry, pc.pos, size);
  }
  sc.power = 0;
}

} // namespace

namespace Systems::InputSystem {
void update(entt::registry &registry, const Keys &keys) {
  const auto view =
      registry.view<ShipComponent, AnimationComponent, InputStateComponent, MotionComponent, PositionComponent>();
  view.each([&](auto &sc, auto &ac, auto &ic, auto &mc, auto &pc) {
    if (keys[ngf::Scancode::Up])
      moveUp(ac, ic, mc);
    if (keys[ngf::Scancode::Down])
      moveDown(ac, ic, mc);
    if (keys[ngf::Scancode::Left])
      moveLeft(mc);
    if (keys[ngf::Scancode::Right])
      moveRight(mc);
    if (!keys[ngf::Scancode::Up] && !keys[ngf::Scancode::Down])
      still(ac, ic, mc);
    if (!keys[ngf::Scancode::Left] && !keys[ngf::Scancode::Right])
      stillRL(mc);
    if (keys[ngf::Scancode::Space])
      shoot(sc);
    else
      noShoot(registry, sc, pc);
  });
}
}
