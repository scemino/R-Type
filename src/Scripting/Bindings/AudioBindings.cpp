#include "AudioBindings.h"
#include <System/Locator.h>
#include <Engine.h>
#include <Scripting/SoundManager.h>

namespace {
void playSound(const std::string &path, std::optional<int> loopTimes) {
  locator::engine::ref().soundManager().playSound(path, loopTimes);
}
}

namespace Bindings {
void bindAudio(sol::state &lua) {
#define ADD_FUNCTION(x) lua.set_function(#x, x)
  ADD_FUNCTION(playSound);
#undef ADD_FUNCTION
}
}