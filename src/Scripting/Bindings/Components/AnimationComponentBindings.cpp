#include <ECS/Entity.h>
#include <ECS/Component/Components.h>
#include "AnimationComponentBindings.h"

namespace {
void setAnim(Entity &e, const std::string &anim, int loop) {
  e.component<AnimationComponent>().setAnim(anim, loop);
}

void setFrame(Entity &e, std::size_t frameIndex) {
  e.component<AnimationComponent>().setFrame(frameIndex);
}

std::size_t getFrame(Entity &e) {
  return e.component<AnimationComponent>().frameIndex;
}

[[nodiscard]] std::string getAnim(const Entity &e) {
  return e.component<AnimationComponent>().current;
}
}

namespace Bindings {
void bindAnimationComponent(sol::state &lua) {
  sol::usertype<Entity> entityType = lua["Entity"];
#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(setAnim);
  ADD_ENTITY_FUNCTION(getAnim);
  ADD_ENTITY_FUNCTION(setFrame);
  ADD_ENTITY_FUNCTION(getFrame);
#undef ADD_ENTITY_FUNCTION
}
}
