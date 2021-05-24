#pragma once

#include <glm/vec2.hpp>

enum class CollisionObject {
  Tile,
  Screen
};

enum CollisionMask {
  CollisionMaskNone = 0x0000,
  CollisionMaskUp = 0x0001,
  CollisionMaskDown = 0x0010,
  CollisionMaskLeft = 0x0100,
  CollisionMaskRight = 0x1000,
};

class CollisionResult final {
public:
  CollisionResult(CollisionMask collisionMask, const glm::ivec2& pos, CollisionObject object)
      : m_collisionMask(collisionMask), m_pos(pos), m_object(object) {}

  [[nodiscard]] bool tile() const { return m_object == CollisionObject::Tile; }
  [[nodiscard]] bool screen() const { return m_object == CollisionObject::Screen; }

  [[nodiscard]] bool up() const { return m_collisionMask & CollisionMaskUp; }
  [[nodiscard]] bool down() const { return m_collisionMask & CollisionMaskDown; }
  [[nodiscard]] bool left() const { return m_collisionMask & CollisionMaskLeft; }
  [[nodiscard]] bool right() const { return m_collisionMask & CollisionMaskRight; }

  [[nodiscard]] glm::ivec2 pos() const { return m_pos; }

private:
  CollisionMask m_collisionMask;
  glm::ivec2 m_pos;
  CollisionObject m_object;
};
