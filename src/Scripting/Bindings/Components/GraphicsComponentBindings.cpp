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

void setZOrder(Entity &e, float zOrder) {
  e.component<GraphicComponent>().zOrder = zOrder;
}

float getZOrder(const Entity &e) {
  return e.component<GraphicComponent>().zOrder;
}

void setFlipX(Entity &e, bool flipX) {
  e.component<GraphicComponent>().flipX = flipX;
}

bool getFlipX(const Entity &e) {
  return e.component<GraphicComponent>().flipX;
}
}

namespace Bindings {
void bindGraphicsComponent(sol::state &lua) {
  sol::usertype<Entity> entityType = lua["Entity"];
#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(setVisible);
  ADD_ENTITY_FUNCTION(getVisible);
  ADD_ENTITY_FUNCTION(setZOrder);
  ADD_ENTITY_FUNCTION(getZOrder);
  ADD_ENTITY_FUNCTION(setFlipX);
  ADD_ENTITY_FUNCTION(getFlipX);
#undef ADD_ENTITY_FUNCTION
}
}
