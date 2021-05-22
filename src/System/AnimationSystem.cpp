#include "AnimationSystem.h"
#include "Component/Components.h"

namespace Systems::AnimationSystem {

void update(entt::registry &registry) {
  registry.view<GraphicComponent, AnimationComponent>()
      .each([&](auto &gc, auto &ac) {
        if (ac.current.empty())
          return;
        const auto &anim = ac.animations.at(ac.current);
        if (ac.delay >= anim.frameDelay) {
          ac.delay = 0;
          ac.frameIndex = (ac.frameIndex + 1) % anim.frames.size();
        } else {
          ++ac.delay;
        }
        gc.frame = anim.frames[ac.frameIndex].rect;
        gc.offset = anim.frames[ac.frameIndex].offset;
        gc.texture = ac.texture;
      });
}

}
