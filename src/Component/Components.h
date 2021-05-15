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

enum class EntityState {
  Alive,
  Explode
};

struct ShipComponent {
  ShipDirection direction{ShipDirection::None};
  EntityState state{EntityState::Alive};
  int frameDelay{0};
  int frameIndex{0};
  int64_t aliveTime{0};
  bool pulsedShot{false};
  int power{0};
  int64_t shotDelay{0};
  int64_t lastShotTime{0};
};

struct ShotComponent {
  int size{0};
  EntityState state{EntityState::Alive};
  int frameIndex{0};
  int frameDelay{6};
};

struct InvincibleComponent {
  int delay{0};
};
