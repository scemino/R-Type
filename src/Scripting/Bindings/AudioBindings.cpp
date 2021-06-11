#include "AudioBindings.h"
#include <Locator.h>
#include <Engine.h>
#include <Scripting/SoundManager.h>

namespace {
void playSound(const std::string& path)
{
  locator::engine::ref().soundManager().playSound(path);
}
}

namespace Bindings {
void bindAudio(sol::state &lua) {
#define ADD_FUNCTION(x) lua.set_function(#x, x)
  ADD_FUNCTION(playSound);
#undef ADD_FUNCTION
}
}