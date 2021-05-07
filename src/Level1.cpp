#include "Engine.h"
#include "Level1.h"

Level1::Level1(Engine *engine, Spaceship *spaceship, const char *mapPath, const char *texturePath)
    : Level(engine, spaceship, mapPath, texturePath) {
}

int Level1::updatePosition() {
  int pos = 0;
  if (m_delay)
    --m_delay;
  else {
    pos = LevelSpeed;
    m_delay = LevelDelay;
  }
  return pos;
}

glm::ivec2 Level1::getRespawnPosition() const {
  return {m_position + 100, 120};
}