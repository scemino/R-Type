#include <glm/vec2.hpp>
#include <ECS/Component/Components.h>
#include <Scripting/EntityManager.h>
#include "PositionComponentBindings.h"

namespace {
void setPosition(Entity &e, const glm::vec2 &position) {
  e.component<PositionComponent>().setPosition(position);
}

glm::vec2 getPosition(const Entity &e) {
  return e.component<PositionComponent>().getPosition();
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
