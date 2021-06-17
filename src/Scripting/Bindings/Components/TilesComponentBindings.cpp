#include <ECS/Component/Components.h>
#include <Scripting/EntityManager.h>
#include "TilesComponentBindings.h"

namespace {
void setTilesData(Entity &e, const std::vector<int> tiles) {
  e.component<TilesComponent>().tiles = tiles;
}
}

namespace Bindings {
void bindTilesComponent(sol::state &lua) {
  sol::usertype<Entity> entityType = lua["Entity"];
#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(setTilesData);
#undef ADD_ENTITY_FUNCTION
}
}
