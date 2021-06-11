#include "Bindings.h"
#include "EntityBindings.h"
#include "MathBindings.h"
#include "PositionComponentBindings.h"
#include "MotionComponentBindings.h"
#include "AnimationComponentBindings.h"
#include "FileSystemBindings.h"
#include "AudioBindings.h"

namespace Bindings {
void bindAll(sol::state &lua) {
  bindFileSystem(lua);
  bindEntity(lua);
  bindMath(lua);
  bindPositionComponent(lua);
  bindMotionComponent(lua);
  bindAnimationComponent(lua);
  bindAudio(lua);
}
}