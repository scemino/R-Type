#pragma once

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <ngf/Graphics/Rect.h>
#include <entt/entt.hpp>

struct PositionComponent {
  glm::vec2 pos;
};

struct MotionComponent {
  glm::vec2 velocity;
};

struct HealthComponent {
  int health{0};
  int lives{0};
};

struct CollideComponent {
  glm::vec2 size;
};

enum class ShipDirection {
  Upper,
  Up,
  None,
  Down,
  MoreDown
};

enum class SpaceshipState {
  Alive,
  Explode,
  Dead
};

struct ShipComponent {
  ShipDirection direction{ShipDirection::None};
  SpaceshipState state{SpaceshipState::Alive};
  int delay{0};
  int64_t aliveTime{0};
  bool pulsedShot{false};
  int power{0};
  int64_t shotDelay{0};
  int64_t lastShotTime{0};
  int64_t lastMagicTime{0};
};

struct ShotComponent {
  int size{0};
  SpaceshipState state{SpaceshipState::Alive};
  int seq{0};
  int delay{6};
};

struct InvincibleComponent {
  int delay{0};
};
