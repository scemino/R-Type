#include "AudioBindings.h"
#include <System/Locator.h>
#include <Engine.h>
#include <Scripting/SoundManager.h>

namespace {
void playSound(const std::string& path)
{
  // disable sound it's buggy :(
  //locator::engine::ref().soundManager().playSound(path);
}
}

namespace Bindings {
void bindAudio(sol::state &lua) {
#define ADD_FUNCTION(x) lua.set_function(#x, x)
  ADD_FUNCTION(playSound);
#undef ADD_FUNCTION
}
}