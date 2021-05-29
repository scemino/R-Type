#include <Engine.h>
#include "EntityFactory.h"
#include "Component/Components.h"
#include "AnimationLoader.h"

namespace EntityFactory {

void createPlayer(EntityManager &manager) {

  auto e = manager.createEntity();
  auto entity = e.getId();
  auto &ac = e.registry().emplace<AnimationComponent>(entity);
  auto pEngine = e.registry().ctx<Engine *>();
  auto animsInfo = loadAnimations(*pEngine, "resources/anims/spaceship.json");;
  ac.animations = animsInfo.animations;
  ac.current = animsInfo.initialAnim;
  ac.playing = true;

  e.registry().emplace<NameComponent>(entity, "fighter");
  e.registry().emplace<PositionComponent>(entity, glm::vec2(32, 60));
  e.registry().emplace<GraphicComponent>(entity);
  e.registry().emplace<InputStateComponent>(entity);
  e.registry().emplace<MotionComponent>(entity);
  e.registry().emplace<ShipComponent>(entity);
  e.registry().emplace<CollideComponent>(entity, glm::vec2(32, 14));
  e.registry().emplace<InvincibleComponent>(entity, 240);
}

}