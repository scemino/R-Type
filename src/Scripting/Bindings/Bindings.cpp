#include "Bindings.h"
#include "EntityBindings.h"
#include "MathBindings.h"
#include "PositionComponentBindings.h"
#include "MotionComponentBindings.h"
#include "AnimationComponentBindings.h"

namespace Bindings
{
void bindAll(sol::state& lua){
  bindEntity(lua);
  bindMath(lua);
  bindPositionComponent(lua);
  bindMotionComponent(lua);
  bindAnimationComponent(lua);
}
}