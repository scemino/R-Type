#include <glm/vec2.hpp>
#include <ECS/Component/Components.h>
#include <Scripting/EntityManager.h>
#include "PositionComponentBindings.h"

namespace {
void addChild(Entity &e, const sol::table &childTable) {
  Entity& child = childTable["cppRef"];
  e.component<HierarchyComponent>().addChild(child);
}

void removeChild(Entity &e, const sol::table &childTable) {
  Entity& child = childTable["cppRef"];
  e.component<HierarchyComponent>().removeChild(child);
}

}

namespace Bindings {
void bindHierarchyComponent(sol::state &lua) {
  sol::usertype<Entity> entityType = lua["Entity"];
#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(addChild);
  ADD_ENTITY_FUNCTION(removeChild);
#undef ADD_ENTITY_FUNCTION
}
}
