#include <ECS/ComponentFactory.h>
#include <entt/entt.hpp>
#include "EntityBindings.h"
#include <System/Locator.h>
#include <Scripting/EntityManager.h>

namespace Bindings {

namespace {
void emplace(const Entity& e, const std::string &name) {
  locator::engine::ref().componentFactory().emplace(e.getId(), name, std::nullopt);
}

void emplace_with_table(const Entity& e, const std::string &name, const sol::table &t) {
  locator::engine::ref().componentFactory().emplace(e.getId(), name, t);
}

void die(const Entity &e) {
  locator::engine::ref().entityManager().destroyEntity(e.getId());
}
}

void bindEntity(sol::state &lua) {

#define ADD_MEMBER_FUNCTION(x) entityType.set_function(#x, &Entity::x)
  auto entityType = lua.new_usertype<Entity>("Entity", sol::call_constructor,
                                             sol::factories([&]() { return locator::engine::ref().entityManager().createEntity(); }));
  ADD_MEMBER_FUNCTION(getId);
#undef ADD_MEMBER_FUNCTION

  entityType.set_function("emplace", sol::overload(&emplace, &emplace_with_table));

#define ADD_ENTITY_FUNCTION(x) entityType.set_function(#x, x)
  ADD_ENTITY_FUNCTION(die);
#undef ADD_ENTITY_FUNCTION
}
}