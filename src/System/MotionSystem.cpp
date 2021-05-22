#include "MotionSystem.h"
#include "Component/Components.h"

namespace Systems::MotionSystem {

void update(entt::registry &registry) {
  registry.view<PositionComponent, MotionComponent>()
      .each([&](auto &pc, const auto &mc) {
        pc.pos += mc.velocity;
      });
}

}