#include <Engine.h>
#include "EntityFactory.h"
#include "Component/Components.h"
#include "AnimationLoader.h"

namespace {
constexpr int shipShot[14][4] = {
    {303, 85, 14, 12}, {288, 87, 11, 9}, {249, 90, 16, 4},
    {249, 90, 16, 4}, {249, 105, 16, 8}, {232, 103, 16, 12},
    {200, 121, 32, 10}, {233, 120, 32, 12}, {168, 137, 48, 12},
    {217, 136, 48, 14}, {136, 154, 64, 14}, {201, 154, 64, 14},
    {104, 171, 80, 14}, {185, 170, 80, 16}};

ngf::irect toRect(const int *v) {
  return ngf::irect::fromPositionSize({v[0], v[1]}, {v[2], v[3]});
}
}

namespace EntityFactory {

entt::entity createPlayer(entt::registry &registry) {

  const entt::entity e = registry.create();
  auto &ac = registry.emplace<AnimationComponent>(e);
  auto pEngine = registry.ctx<Engine *>();
  auto animsInfo = loadAnimations(*pEngine, "resources/anims/spaceship.json");;
  ac.animations = animsInfo.animations;
  ac.current = animsInfo.initialAnim;

  registry.emplace<NameComponent>(e, "fighter");
  registry.emplace<PositionComponent>(e, glm::vec2(32, 60));
  registry.emplace<HealthComponent>(e, 100, 3);
  registry.emplace<GraphicComponent>(e);
  registry.emplace<InputStateComponent>(e);
  registry.emplace<MotionComponent>(e);
  registry.emplace<ShipComponent>(e);
  registry.emplace<CollideComponent>(e, glm::vec2(32, 14));
  registry.emplace<InvincibleComponent>(e, 240);

  return e;
}

entt::entity createSpaceshipShot(entt::registry &registry, const glm::vec2 &pos, int size) {
  const entt::entity e = registry.create();
  registry.emplace<NameComponent>(e, "fighter shot");
  registry.emplace<GraphicComponent>(e);

  auto engine = registry.ctx<Engine *>();

  int numSeq = 2 + (2 * size);
  std::vector<AnimationFrame> frames{
      AnimationFrame{ngf::irect::fromPositionSize({shipShot[numSeq][0], shipShot[numSeq][1]},
                                                  {shipShot[numSeq][2], shipShot[numSeq][3]})},
      AnimationFrame{
          ngf::irect::fromPositionSize({shipShot[numSeq + 1][0], shipShot[numSeq + 1][1]},
                                       {shipShot[numSeq + 1][2], shipShot[numSeq + 1][3]})
      }};

  auto texture = engine->loadTexture("resources/images/r-typesheet1.png");
  auto &ac = registry.emplace<AnimationComponent>(e);
  ac.animations["idle"] = Animation{frames, texture, 6};
  ac.current = "idle";

  registry.emplace<PositionComponent>(e, glm::vec2{pos.x + 32.f, pos.y});
  registry.emplace<CollideComponent>(e, glm::vec2(14, 12));

  // shot motion
  const auto speedX = shipShot[0][2] - size - (size >> 1);
  registry.emplace<MotionComponent>(e, glm::vec2(speedX, 0));
  return e;
}

entt::entity createEnemy1(entt::registry &registry, const glm::vec2 &pos) {
  const entt::entity e = registry.create();
  registry.emplace<PositionComponent>(e, pos);
  registry.emplace<GraphicComponent>(e);

  auto engine = registry.ctx<Engine *>();
  auto &ac = registry.emplace<AnimationComponent>(e);
  auto animsInfo = loadAnimations(*engine, "resources/anims/enemy1.json");
  ac.animations = animsInfo.animations;
  ac.current = animsInfo.initialAnim;

  registry.emplace<MotionComponent>(e, glm::vec2(-3, 0));
  registry.emplace<CollideComponent>(e);
  return e;
}

}