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

//void Level1::generateEnemies() {
//  auto rect = getRect();

//  if (m_position == (75 * TileWidth - GAME_WIDTH) && !m_delay) {
//    auto *enemy = new Enemy1(m_engine, {rect.max.x + 8, 36});
//    addEnemy(enemy);
//  }
//
//  if (m_position == (77 * TileWidth - GAME_WIDTH) && !m_delay) {
//    auto enemy = new Enemy1(m_engine, {rect.max.x + 8, 24});
//    addEnemy(enemy);
//  }
//
//  if (m_position == (80 * TileWidth - GAME_WIDTH) && !m_delay) {
//    auto enemy = new Enemy1(m_engine, {rect.max.x + 8, 48});
//    addEnemy(enemy);
//  }
//
//  if (m_position == (82 * TileWidth - GAME_WIDTH) && !m_delay) {
//    auto enemy = new Enemy1(m_engine, {rect.max.x + 8, 60});
//    addEnemy(enemy);
//  }
//
//  if (m_position == (88 * TileWidth - GAME_WIDTH) && !m_delay) {
//    auto *enemy = new Enemy1(m_engine, {rect.max.x + 8, 176});
//    addEnemy(enemy);
//  }
//
//  if (m_position == (90 * TileWidth - GAME_WIDTH) && !m_delay) {
//    auto enemy = new Enemy1(m_engine, {rect.max.x + 8, 164});
//    addEnemy(enemy);
//  }
//
//  if (m_position == (93 * TileWidth - GAME_WIDTH) && !m_delay) {
//    auto enemy = new Enemy1(m_engine, {rect.max.x + 8, 188});
//    addEnemy(enemy);
//  }
//
//  if (m_position == (95 * TileWidth - GAME_WIDTH) && !m_delay) {
//    auto enemy = new Enemy1(m_engine, {rect.max.x + 8, 200});
//    addEnemy(enemy);
//  }
//
//  if (m_position == (100 * TileWidth - GAME_WIDTH) && !m_delay) {
//    auto enemy = new Enemy2(m_engine, {rect.max.x + 8, 200});
//    addEnemy(enemy);
//  }

//}