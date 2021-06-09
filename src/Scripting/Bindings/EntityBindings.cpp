#include <Scripting/Entity.h>
#include <entt/entt.hpp>
#include "EntityBindings.h"
#include "Locator.h"

namespace Bindings {

namespace {
void remove(const Entity &e) {
  locator::engine::ref().entityManager().removeEntity(e.getId());
}
}

void bindEntity(sol::state &lua) {

#define ADD_MEMBER_FUNCTION(x) entityType.set_function(#x, &Entity::x)
  auto entityType = lua.new_usertype<Entity>("Entity", sol::call_constructor,
                                             sol::factories([&]() { return locator::engine::ref().entityManager().createEntity(); }));
  ADD_MEMBER_FUNCTION(emplace);
  ADD_MEMBER_FUNCTION(getId);
#undef ADD_MEMBER_FUNCTION

#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(remove);
#undef ADD_ENTITY_FUNCTION
}
}