#include <imgui.h>
#include "DebugTools.h"
#include "Component/Components.h"

namespace DebugTools {
void show(entt::registry &registry) {
  registry.each([&registry](auto e) {
    std::string name;
    auto nc = registry.try_get<NameComponent>(e);
    name = nc ? nc->name : "unknown";
    if (ImGui::TreeNode(name.c_str())) {
      auto pc = registry.try_get<PositionComponent>(e);
      if (pc) {
        ImGui::DragFloat2("Position", &pc->pos.x);
      }
      auto mc = registry.try_get<MotionComponent>(e);
      if (mc) {
        ImGui::DragFloat2("Velocity", &mc->velocity.x);
      }
      auto hc = registry.try_get<HealthComponent>(e);
      if (hc) {
        ImGui::DragInt("Health", &hc->health);
        ImGui::DragInt("Lives", &hc->lives);
      }
      auto cc = registry.try_get<CollideComponent>(e);
      if (cc) {
        ImGui::DragFloat2("Hit box", &cc->size.x);
      }
      auto ic = registry.try_get<InvincibleComponent>(e);
      if (ic) {
        ImGui::DragInt("Invicible delay", &ic->delay);
      }
      ImGui::TreePop();
    }
  });
}
}
