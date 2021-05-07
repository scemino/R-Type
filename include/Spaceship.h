#pragma once

#include "Entity.h"
#include <vector>

enum class SpaceshipState {
  Alive,
  Explode,
  Dead
};

class Engine;
class Keys;
class Shield;

class Spaceship : public Entity {
public:
  explicit Spaceship(Engine *engine);
  ~Spaceship() override;

  [[nodiscard]] int getLives() const;
  [[nodiscard]] int getMagic() const;
  [[nodiscard]] bool canShootMagic() const;
  [[nodiscard]] int loadShot() const;

  void spawn(const glm::ivec2& pos);
  void die() override;
  [[nodiscard]] ngf::irect getRect() const override;
  void offset(int x, int y) override;

  void processKeys(const Keys &keys);
  void update() override;
  void draw(ngf::RenderTarget &target, ngf::RenderStates states) const override;

private:
  void moveUp();
  void moveDown();
  void moveRight();
  void moveLeft();
  void still();

  void shoot();
  void noShoot();
  void shootMagic();

  void reset();

private:
  SpaceshipState m_state{SpaceshipState::Alive};
  int m_lives;                // how many lives are left
  int m_power;                // how many power the ship has left
  int m_magic;                // how many magic are left
  int64_t m_aliveTime;        // how long the ship has been alive

  int64_t m_shotDelay;
  bool m_pulsedShot;
  int64_t m_lastShot;        // when shot for the last time
  int64_t m_lastMagic;
  int m_shotLoad;

  float m_pixelsMove;

  float m_xF, m_yF;
  glm::ivec2 m_lastPos;

  std::shared_ptr<ngf::Texture> m_texSpaceship;
  int m_seq{0};
};
