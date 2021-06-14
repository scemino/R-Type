#include <glm/vec2.hpp>
#include <ECS/Component/Components.h>
#include <Scripting/EntityManager.h>
#include "PositionComponentBindings.h"

namespace {
void addChild(Entity &e, Entity &child) {
  e.component<HierarchyComponent>().addChild(child);
}
}

namespace Bindings {
void bindHierarchyComponent(sol::state &lua) {
  sol::usertype<Entity> entityType = lua["Entity"];
#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(addChild);
#undef ADD_ENTITY_FUNCTION
}
}
