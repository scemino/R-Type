#pragma once

#include "Level.h"

class Level1 final : public Level {
public:
  Level1(Engine *engine, Spaceship *spaceship, const char *mapPath, const char *texturePath);

  [[nodiscard]] glm::ivec2 getRespawnPosition() const final;

private:
  int updatePosition() final;
//  void generateEnemies() final;
};