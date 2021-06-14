#include <ECS/Component/Components.h>
#include <Scripting/EntityManager.h>
#include "GraphicsComponentBindings.h"

namespace {
void setVisible(Entity &e, bool visible) {
  e.component<GraphicComponent>().visible = visible;
}

bool getVisible(const Entity &e) {
  return e.component<GraphicComponent>().visible;
}
}

namespace Bindings {
void bindGraphicsComponent(sol::state &lua) {
  sol::usertype<Entity> entityType = lua["Entity"];
#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(setVisible);
  ADD_ENTITY_FUNCTION(getVisible);
#undef ADD_ENTITY_FUNCTION
}
}
