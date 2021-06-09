#include <Scripting/Entity.h>
#include <Component/Components.h>
#include "AnimationComponentBindings.h"

namespace {
void setAnim(Entity &e, const std::string &anim, int loop) {
  e.component<AnimationComponent>().setAnim(anim, loop);
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
#undef ADD_ENTITY_FUNCTION
}
}
