#include "MotionSystem.h"
#include "Component/Components.h"

namespace MotionSystem {

void update(entt::registry &registry) {
  const auto motionView = registry.view<PositionComponent, MotionComponent>();
  for (const entt::entity e : motionView) {
    auto &pc = motionView.get<PositionComponent>(e);
    const auto &mc = motionView.get<MotionComponent>(e);
    pc.pos += mc.velocity;
  }
}

}