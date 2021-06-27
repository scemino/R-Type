#include "LevelBindings.h"
#include <System/Locator.h>
#include <Level.h>

namespace {
void setLevelPosition(int position) {
  locator::engine::ref().level().setPosition(position);
}
int getLevelPosition() {
  return locator::engine::ref().level().getPosition();
}

int map(const glm::vec2 &pos) {
  return locator::engine::ref().level().get(pos);
}

const std::vector<LevelObject>& getLevelObjs() {
  return locator::engine::ref().level().getLevelObjects();
}
}

namespace Bindings {
void bindLevel(sol::state &lua) {
  lua.new_usertype<LevelObject>("lvlObj",
                              "name", &LevelObject::name,
                              "pos", &LevelObject::pos,
                              "group", &LevelObject::group,
                              sol::meta_function::to_string,
                              [](const LevelObject &o) -> std::string {
                                return "name: " + o.name + ", pos: (" + std::to_string(o.pos.x) + ','
                                    + std::to_string(o.pos.y)
                                    + ','
                                    + std::to_string(o.group) + ')';
                              });
#define ADD_FUNCTION(x) lua.set_function(#x, x)
  ADD_FUNCTION(setLevelPosition);
  ADD_FUNCTION(getLevelPosition);
  ADD_FUNCTION(map);
  ADD_FUNCTION(getLevelObjs);
#undef ADD_FUNCTION
}
}