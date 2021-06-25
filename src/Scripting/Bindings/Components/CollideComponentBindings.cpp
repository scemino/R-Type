#include <ECS/Component/Components.h>
#include <Scripting/EntityManager.h>
#include "CollideComponentBindings.h"

namespace {
glm::vec2 getHitbox(const Entity &e) {
  return e.component<CollideComponent>().hitbox.getSize();
}
}

namespace Bindings {
void bindCollideComponent(sol::state &lua) {
  sol::usertype<Entity> entityType = lua["Entity"];
#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(getHitbox);
#undef ADD_ENTITY_FUNCTION
}
}
