#include <ngf/IO/Json/JsonParser.h>
#include "InitSystem.h"
#include "Engine.h"
#include "Component/Components.h"

namespace Systems::InitSystem {

void update(entt::registry &registry) {

  auto engine = registry.ctx<Engine *>();
  const auto view = registry.view<EntityComponent, AnimationComponent>();

  for (const entt::entity e : view) {
    const auto &ec = view.get<EntityComponent>(e);
    auto &ac = view.get<AnimationComponent>(e);
    const auto jAnims = ngf::Json::load(ec.filename);
    ac.texture = engine->loadTexture(jAnims["texture"].getString());
    for (const auto &jAnimItem : jAnims["animations"].items()) {
      const auto &name = jAnimItem.key();
      const auto &jAnim = jAnimItem.value();
      const auto &jFrames = jAnim["frames"];
      const auto &jDelay = jAnim["delay"];
      const auto delay = jDelay.isNull() ? 6 : jDelay.getInt();
      std::vector<AnimationFrame> frames;
      for (const auto &jFrame : jFrames) {
        frames.push_back(AnimationFrame{ngf::irect::fromPositionSize({
                                                                         jFrame[0].getInt(), jFrame[1].getInt()},
                                                                     {jFrame[2].getInt(), jFrame[3].getInt()})});
      }
      ac.animations[name] = Animation{frames, delay};
    }

    ac.current = "idle";
  }
}
}
