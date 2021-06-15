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
    const Animation& anim = it->second;
    // skip if animation is not playing
    if (!ac.playing)
      continue;
    // check if frame is elapsed
    if (ac.delay >= anim.frameDelay) {
      ac.delay = 0;
      ac.frameIndex++;
      if (ac.frameIndex >= anim.frames.size()) {
        if (ac.loop > 0) {
          ac.loop--;
        }
        // end of animation ?
        if (ac.loop == 0) {
          ac.playing = false;
          // notify end of animation
          auto& engine = locator::engine::ref();
          auto& entity = engine.entityManager().getEntity(e);
          engine.eventManager().publish(entity, "anim", "name", ac.current, "eventType", "finished");
          continue;
        }
        ac.frameIndex = anim.loopFrom;
      }
    } else {
      ++ac.delay;
    }
    gc.frame = anim.frames[ac.frameIndex].rect;
    gc.origin = anim.frames[ac.frameIndex].origin;
    gc.texture = anim.texture;
  }
}
}
