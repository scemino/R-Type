#pragma once

#include <glm/vec2.hpp>
#include "Entity.h"

enum class ShotState {
  Alive,
  Explosion,
  Dead
};

class Engine;

class Shot : public Entity {
public:
  explicit Shot(Engine *engine) : Entity(engine) {}
  Shot(Engine *engine, const glm::ivec2 &pos) : Entity(engine, pos) {}
  ~Shot() override = default;

  [[nodiscard]] bool isBad() const { return m_isBad; }
  [[nodiscard]] int getDamage() const { return m_damage; }

  [[nodiscard]] bool isAlive() const { return m_state == ShotState::Alive; }

  virtual void explode() {}

protected:
  ShotState m_state{ShotState::Alive};
  bool m_isBad{true}; // true if it has been shot by an enemy
  int m_damage{0};     // how much life takes away
};
