#include "InvincibleSystem.h"
#include "Component/Components.h"

namespace InvincibleSystem {

void update(entt::registry &registry) {
  const auto view = registry.view<InvincibleComponent>();
  for (const entt::entity e : view) {
    auto &ic = view.get<InvincibleComponent>(e);
    ic.delay--;
    if (ic.delay == 0) {
      registry.remove<InvincibleComponent>(e);
    }
  }
}
}