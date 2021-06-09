#include <glm/vec2.hpp>
#include <Scripting/Entity.h>
#include <Component/Components.h>
#include "PositionComponentBindings.h"

namespace {
void setPosition(Entity &e, const glm::vec2 &position) {
  e.component<PositionComponent>().pos = position;
}

glm::vec2 getPosition(const Entity &e) {
  return e.component<PositionComponent>().pos;
}
}

namespace Bindings {
void bindPositionComponent(sol::state &lua) {
  sol::usertype<Entity> entityType = lua["Entity"];
#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(setPosition);
  ADD_ENTITY_FUNCTION(getPosition);
#undef ADD_ENTITY_FUNCTION
}
}
