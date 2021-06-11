#include "CameraSystem.h"
#include "Level.h"
#include "Component/Components.h"

namespace Systems::CameraSystem {

void update(entt::registry &registry) {
  auto pLevel = registry.ctx<Level *>();
  auto view = registry.view<CameraComponent, PositionComponent>();
  glm::vec2 vel, pos;
  for (entt::entity e : view) {
    const auto &pc = view.get<PositionComponent>(e);
    pos = pc.pos;
    break;
  }

  if (pLevel)
    pLevel->setPosition(static_cast<int>(pos.x));
}

}