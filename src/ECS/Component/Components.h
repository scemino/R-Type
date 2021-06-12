#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <glm/vec2.hpp>
#include <ngf/Graphics/Rect.h>
#include <ngf/Graphics/Texture.h>
#include <entt/entt.hpp>
#include <sol/sol.hpp>
#include <CollisionResult.h>
#include <Graphics/Animation.h>

struct PositionComponent {
  glm::vec2 pos{};

  PositionComponent() = default;
  explicit PositionComponent(const glm::vec2 &p) : pos(p) {}
  explicit PositionComponent(const sol::table &t);
};

struct MotionComponent {
  glm::vec2 velocity{};

  MotionComponent() = default;
  explicit MotionComponent(const glm::vec2 &v) : velocity(v) {}
  explicit MotionComponent(const sol::table &t);
};

struct CollideComponent {
  glm::ivec2 size{};

  CollideComponent() = default;
  explicit CollideComponent(const glm::vec2 &s) : size(s) {}
  explicit CollideComponent(const sol::table &t);
};

struct GraphicComponent {
  std::shared_ptr<ngf::Texture> texture;
  ngf::irect frame;
  glm::vec2 offset{};
  bool visible{true};

  GraphicComponent() = default;
  explicit GraphicComponent(const sol::table &t);
};

struct AnimationComponent {
  std::string current;
  std::unordered_map<std::string, Animation> animations;
  std::size_t frameIndex{0};
  int loop{1};
  int delay{0};
  bool playing{false};

  AnimationComponent() = default;
  explicit AnimationComponent(const sol::table &t);

  void setAnim(const std::string &anim, int loop);
};

struct NameComponent {
  std::string name;

  NameComponent() = default;
  explicit NameComponent(std::string  n) : name(std::move(n)) {}
  explicit NameComponent(const sol::table &t);
};
