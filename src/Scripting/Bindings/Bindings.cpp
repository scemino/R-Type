#include "Bindings.h"
#include "EntityBindings.h"
#include "MathBindings.h"
#include "Components/PositionComponentBindings.h"
#include "Components/MotionComponentBindings.h"
#include "Components/AnimationComponentBindings.h"
#include "Components/NameComponentBindings.h"
#include "Components/HierarchyComponentBindings.h"
#include "Components/GraphicsComponentBindings.h"
#include "FileSystemBindings.h"
#include "AudioBindings.h"
#include "LevelBindings.h"

namespace Bindings {
void bindAll(sol::state &lua) {
  // general bindings
  bindFileSystem(lua);
  bindMath(lua);
  bindAudio(lua);
  bindLevel(lua);

  // entity bindings
  bindEntity(lua);

  // components bindings
  bindPositionComponent(lua);
  bindMotionComponent(lua);
  bindAnimationComponent(lua);
  bindNameComponent(lua);
  bindHierarchyComponent(lua);
  bindGraphicsComponent(lua);
}
}