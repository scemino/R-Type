#pragma once
#include <cmath>
#include <ngf/Graphics/Rect.h>
#include <glm/vec2.hpp>
#include <ngf/Graphics/RenderStates.h>
#include <ngf/Graphics/RenderTarget.h>

class Engine;

class Entity {
public:
  explicit Entity(Engine *pEngine);
  Entity(Engine *pEngine, glm::ivec2 pos);

  virtual ~Entity() = default;

  [[nodiscard]] glm::ivec2 getPosition() const;
  void setPosition(glm::ivec2 pos);
  virtual void offset(int x, int y);

  [[nodiscard]] bool isDead() const;
  virtual void die() {}

  [[nodiscard]] virtual ngf::irect getRect() const { return ngf::irect(); }
  [[nodiscard]] virtual bool collide(const ngf::irect &rect) const { return false; }

  virtual void update() {}
  virtual void draw(ngf::RenderTarget &target, ngf::RenderStates states) const {}

protected:
  int m_delay{0};
  glm::ivec2 m_pos{0, 0};
  Engine *m_engine{nullptr};
  bool m_dead{false};
};
