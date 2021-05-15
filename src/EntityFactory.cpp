#include "EntityFactory.h"
#include "Component/Components.h"

namespace {
constexpr int shipShot[14][4] = {
    {303, 85, 14, 12}, {288, 87, 11, 9}, {249, 90, 16, 4},
    {249, 90, 16, 4}, {249, 105, 16, 8}, {232, 103, 16, 12},
    {200, 121, 32, 10}, {233, 120, 32, 12}, {168, 137, 48, 12},
    {217, 136, 48, 14}, {136, 154, 64, 14}, {201, 154, 64, 14},
    {104, 171, 80, 14}, {185, 170, 80, 16}};
constexpr int shipShotMid[6] = {91, 108, 125, 142, 160, 177};
}

namespace EntityFactory {

entt::entity createPlayer(entt::registry &registry) {
  const entt::entity e = registry.create();
  registry.emplace<PositionComponent>(e, glm::vec2(32, 60));
  registry.emplace<HealthComponent>(e, 100, 3);
  registry.emplace<ShipComponent>(e);
  registry.emplace<MotionComponent>(e);
  registry.emplace<CollideComponent>(e, glm::vec2(32, 14));
  registry.emplace<InvincibleComponent>(e, 240);
  return e;
}

entt::entity createSpaceshipShot(entt::registry &registry, const glm::vec2 &pos, int size) {
  const entt::entity e = registry.create();
  registry.emplace<PositionComponent>(e, glm::vec2{pos.x + 32.f, pos.y});
  registry.emplace<CollideComponent>(e, glm::vec2(14, 12));
  registry.emplace<ShotComponent>(e, size);

  // shot motion
  const auto speedX = shipShot[0][2] - size - (size >> 1);
  registry.emplace<MotionComponent>(e, glm::vec2(speedX, 0));
  return e;
}

}