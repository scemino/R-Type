#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/vec2.hpp>
#include <ngf/Graphics/Rect.h>
#include <ngf/Graphics/Texture.h>
#include <entt/entt.hpp>
#include <CollisionResult.h>

struct PositionComponent {
  glm::vec2 pos;
};

struct MotionComponent {
  glm::vec2 velocity;
};

struct CollideComponent {
  glm::vec2 size;
};

struct InputStateComponent {
  bool up;
  bool down;
  int delay;
};

struct ShipComponent {
  int frameDelay{0};
  int frameIndex{0};
  int64_t aliveTime{0};
  bool pulsedShot{false};
  int power{0};
  int64_t shotDelay{0};
  int64_t lastShotTime{0};
};

struct GraphicComponent {
  std::shared_ptr<ngf::Texture> texture;
  ngf::irect frame;
  glm::vec2 offset;
  bool visible{true};
};

struct AnimationFrame {
  ngf::irect rect;
  glm::vec2 offset{0};
};

struct Animation {
  std::vector<AnimationFrame> frames;
  std::shared_ptr<ngf::Texture> texture;
  int frameDelay{0};
};

struct AnimationComponent {
  std::string current;
  std::unordered_map<std::string, Animation> animations;
  std::size_t frameIndex{0};
  int loop{1};
  int delay{0};
  bool playing{false};
};

struct InvincibleComponent {
  int delay{0};
};

struct NameComponent {
  std::string name;
};

struct CameraComponent {};
