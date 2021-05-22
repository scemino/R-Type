#include "ExplodeSystem.h"
#include "Component/Components.h"

namespace Systems::ExplodeSystem {
void update(entt::registry &registry) {
  const auto view = registry.view<ExplodeComponent, AnimationComponent, HealthComponent>();
  for (const entt::entity e : view) {
    auto &ec = view.get<ExplodeComponent>(e);
    auto &ac = view.get<AnimationComponent>(e);
    auto &hc = view.get<HealthComponent>(e);
    if (ec.delay == 0) {
      registry.remove<MotionComponent>(e);
      ac.current = "explode";
      ec.delay++;
    } else if (ec.delay == 6 * 8) {
      registry.emplace_or_replace<InvincibleComponent>(e, 240);
      registry.remove<ExplodeComponent>(e);
      registry.emplace<CollideComponent>(e, glm::vec2(32, 14));
      registry.emplace<MotionComponent>(e);
      hc.health = 100;
      ac.current = "idle";
    } else {
      ++ec.delay;
    }
  }
}
}