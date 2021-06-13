#include "MotionComponentBindings.h"
#include <glm/vec2.hpp>
#include <ECS/Entity.h>
#include <ECS/Component/Components.h>

namespace {
void setVelocity(Entity &e, const glm::vec2 &velocity) {
  e.component<MotionComponent>().velocity = velocity;
}

glm::vec2 getVelocity(const Entity &e) {
  return e.component<MotionComponent>().velocity;
}
}

namespace Bindings {
void bindMotionComponent(sol::state &lua) {
  sol::usertype<Entity> entityType = lua["Entity"];
#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(setVelocity);
  ADD_ENTITY_FUNCTION(getVelocity);
#undef ADD_ENTITY_FUNCTION
}
}
