#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/vec2.hpp>
#include <ngf/Graphics/Rect.h>
#include <ngf/Graphics/Texture.h>
#include <entt/entt.hpp>
#include <sol/sol.hpp>
#include <CollisionResult.h>

struct PositionComponent {
  glm::vec2 pos;

  PositionComponent() {}
  PositionComponent(const glm::vec2 &p) : pos(p) {}
  PositionComponent(const sol::table &t);
};

struct MotionComponent {
  glm::vec2 velocity;

  MotionComponent() {}
  MotionComponent(const glm::vec2 &v) : velocity(v) {}
  MotionComponent(const sol::table &t);
};

struct CollideComponent {
  glm::ivec2 size;

  CollideComponent() {}
  CollideComponent(const glm::vec2 &s) : size(s) {}
  CollideComponent(const sol::table &t);
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

  GraphicComponent() {}
  GraphicComponent(const sol::table &t);
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

  AnimationComponent() {}
  AnimationComponent(const sol::table &t);

  void setAnim(const std::string &anim, int loop);
};

struct InvincibleComponent {
  int delay{0};
};

struct NameComponent {
  std::string name;

  NameComponent() {}
  NameComponent(const std::string& n) : name(n) {}
  NameComponent(const sol::table &t);
};

struct CameraComponent {};
