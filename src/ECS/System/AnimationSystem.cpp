#include "AnimationSystem.h"
#include <ECS/Component/Components.h>
#include <System/Locator.h>
#include <System/Log.h>
#include <Scripting/EntityManager.h>
#include <Scripting/EventManager.h>

namespace Systems::AnimationSystem {

void update(entt::registry &registry) {
  const auto view = registry.view<GraphicComponent, AnimationComponent>();
  for (const entt::entity e : view) {
    auto &gc = registry.get<GraphicComponent>(e);
    auto &ac = registry.get<AnimationComponent>(e);
    if (ac.current.empty())
      return;
    auto it = ac.animations.find(ac.current);
    if (it == ac.animations.end()) {
      RTYPE_LOG_WARN("Animation {} not found", ac.current);
      continue;
    }
    // skip if animation is not playing
    if (!ac.playing)
      continue;
    // check if frame is elapsed
    if (ac.delay >= it->second.frameDelay) {
      ac.delay = 0;
      ac.frameIndex++;
      if (ac.frameIndex >= it->second.frames.size()) {
        if (ac.loop > 0) {
          ac.loop--;
        }
        // end of animation ?
        if (ac.loop == 0) {
          ac.playing = false;
          // notify end of animation
          auto& engine = locator::engine::ref();
          auto pEntity = engine.entityManager().getEntity(e);
          engine.eventManager().publish(pEntity, "anim", "name", ac.current, "eventType", "finished");
          continue;
        }
        ac.frameIndex = 0;
      }
    } else {
      ++ac.delay;
    }
    gc.frame = it->second.frames[ac.frameIndex].rect;
    gc.origin = it->second.frames[ac.frameIndex].origin;
    gc.texture = it->second.texture;
  }
}
}
