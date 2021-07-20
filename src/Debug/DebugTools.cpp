#include <imgui.h>
#include <ECS/Component/Components.h>
#include "DebugTools.h"

namespace DebugTools {
void show(entt::registry &registry) {
  if(ImGui::TreeNode("Entities")) {
    registry.each([&registry](auto e) {
      std::string name;
      auto nc = registry.try_get<NameComponent>(e);
      name = nc ? nc->name : "unknown";
      ImGui::PushID(static_cast<int>(e));
      if (ImGui::TreeNode(name.c_str())) {
        auto pc = registry.try_get<PositionComponent>(e);
        if (pc) {
          auto pos = pc->getPosition();
          if (ImGui::DragFloat2("Position", &pos.x)) {
            pc->setPosition(pos);
          }
        }
        auto mc = registry.try_get<MotionComponent>(e);
        if (mc) {
          ImGui::DragFloat2("Velocity", &mc->velocity.x);
        }
        auto cc = registry.try_get<CollideComponent>(e);
        if (cc) {
          ImGui::DragFloat4("Hit box", &cc->hitbox.min.x);
        }
        auto ac = registry.try_get<AnimationComponent>(e);
        if (ac) {
          ImGui::TextDisabled("Animation: %s", ac->current.c_str());
          ImGui::TextDisabled("Frame: %lu", ac->frameIndex);
          ImGui::TextDisabled("Loop: %d", ac->loop);
        }
        ImGui::TreePop();
      }
      ImGui::PopID();
    });
    ImGui::TreePop();
  }

  auto& io = ImGui::GetIO();
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}
}
