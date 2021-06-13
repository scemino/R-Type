#include <imgui.h>
#include <ECS/Component/Components.h>
#include "DebugTools.h"

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
      auto cc = registry.try_get<CollideComponent>(e);
      if (cc) {
        ImGui::DragInt2("Hit box", &cc->size.x);
      }
      auto ac = registry.try_get<AnimationComponent>(e);
      if (ac) {
        ImGui::TextDisabled("Animation: %s", ac->current.c_str());
        ImGui::TextDisabled("Frame: %lu", ac->frameIndex);
        ImGui::TextDisabled("Loop: %d", ac->loop);
      }
      ImGui::TreePop();
    }
  });
}
}
