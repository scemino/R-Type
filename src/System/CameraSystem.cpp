#include "CameraSystem.h"
#include "Level.h"
#include "Component/Components.h"

namespace Systems::CameraSystem {

void update(entt::registry &registry) {
  auto pLevel = registry.ctx<Level *>();
  auto view = registry.view<CameraComponent, PositionComponent, MotionComponent>();
  glm::vec2 vel, pos;
  for (entt::entity e : view) {
    const auto &[mc, pc] = view.get<MotionComponent, PositionComponent>(e);
    vel = mc.velocity;
    pos = pc.pos;
    break;
  }

  if (pLevel)
    pLevel->setPosition(pos.x);

  auto viewOthers = registry.view<MotionComponent, PositionComponent>();
  for (entt::entity e : viewOthers) {
    auto cc = registry.any_of<CameraComponent>(e);
    if (cc)
      continue;
    registry.patch<PositionComponent>(e, [&](auto &pc) { pc.pos.x += vel.x; });
  }
}

}