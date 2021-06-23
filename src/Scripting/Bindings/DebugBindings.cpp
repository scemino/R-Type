#include "DebugBindings.h"
#include <System/Locator.h>
#include <Engine.h>
#include <Scripting/DebugManager.h>

namespace {
void showHitboxes(bool show)
{
  locator::engine::ref().debugManager().showHitboxes = show;
}
}

namespace Bindings {
void bindDebug(sol::state &lua) {
#define ADD_FUNCTION(x) lua.set_function(#x, x)
  ADD_FUNCTION(showHitboxes);
#undef ADD_FUNCTION
}
}