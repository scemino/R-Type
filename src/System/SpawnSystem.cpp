#include "EntityFactory.h"
#include "Engine.h"
#include "Level.h"
#include "SpawnSystem.h"

namespace Systems::SpawnSystem {

void update(entt::registry &registry) {
  return;
  auto level = registry.ctx<Level *>();
  const auto pos = level->getScrollPosition();
  if (pos % 60 == 0) {
    EntityFactory::createEnemy1(registry, glm::vec2{pos + GAME_WIDTH, 60});
  } else if (pos == 70) {
    EntityFactory::createEnemy1(registry, glm::vec2{pos + GAME_WIDTH, 100});
  } else if (pos == 80) {
    EntityFactory::createEnemy1(registry, glm::vec2{pos + GAME_WIDTH, 120}
    );
  }
}

}