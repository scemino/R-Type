#include "InvincibleSystem.h"
#include "Component/Components.h"

namespace Systems::InvincibleSystem {

void update(entt::registry &registry) {
  const auto view = registry.view<InvincibleComponent, GraphicComponent>();
  for (const entt::entity e : view) {
    auto &ic = view.get<InvincibleComponent>(e);
    auto &gc = view.get<GraphicComponent>(e);
    --ic.delay;

    gc.visible = ic.delay % 6;

    if (ic.delay == 0) {
      gc.visible = true,
          registry.remove<InvincibleComponent>(e);
    }
  }
}
}