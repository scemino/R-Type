#pragma once

#include <glm/vec2.hpp>
#include "Shot.h"

enum class SpaceshipShotType {
  Normal,
  Circular
};

class SpaceshipShot final : public Shot {
public:
  SpaceshipShot(Engine *engine, glm::ivec2 pos, SpaceshipShotType type, int size);
  ~SpaceshipShot() final;

  [[nodiscard]] bool collide(const ngf::irect &rect) const final;
  [[nodiscard]] ngf::irect getRect() const final;

  void explode() final;
  void die() final;

  void update() final;
  void draw(ngf::RenderTarget &target, ngf::RenderStates states) const final;

private:
  SpaceshipShotType m_type{SpaceshipShotType::Normal};
  int m_size{0};
  int m_pixelsAdvance;
  std::shared_ptr<ngf::Texture> m_texNave1;
  std::shared_ptr<ngf::Texture> m_texNave2;
  int m_seq{0};
};
