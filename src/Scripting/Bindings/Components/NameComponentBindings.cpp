#include <glm/vec2.hpp>
#include <ECS/Entity.h>
#include <ECS/Component/Components.h>
#include "NameComponentBindings.h"

namespace {
void setName(Entity &e, const std::string& name) {
  e.component<NameComponent>().name = name;
}

std::string getName(const Entity &e) {
  return e.component<NameComponent>().name;
}
}

namespace Bindings {
void bindNameComponent(sol::state &lua) {
  sol::usertype<Entity> entityType = lua["Entity"];
#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(setName);
  ADD_ENTITY_FUNCTION(getName);
#undef ADD_ENTITY_FUNCTION
}
}
