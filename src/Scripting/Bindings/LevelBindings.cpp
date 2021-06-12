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
}

namespace Bindings {
void bindLevel(sol::state &lua) {
#define ADD_FUNCTION(x) lua.set_function(#x, x)
  ADD_FUNCTION(setLevelPosition);
  ADD_FUNCTION(getLevelPosition);
#undef ADD_FUNCTION
}
}