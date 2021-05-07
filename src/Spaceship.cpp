#include "Spaceship.h"
#include "Engine.h"
#include "Level.h"
#include "Keys.h"
#include "SpaceshipShot.h"
#include <ngf/Graphics/Sprite.h>

namespace {
constexpr int SpaceshipMoreDown = 0;
constexpr int SpaceshipDown = 1;
constexpr int SpaceshipSide = 2;
constexpr int SpaceshipUp = 3;
constexpr int SpaceshipUpper = 4;

constexpr int SpaceshipExplosionFrames = 8;

constexpr int SpaceshipMoveDelay = 6;
constexpr int SpaceshipExplosionDelay = 6;

constexpr int SpaceshipMovePixels = 3.0f;

constexpr int SpaceshipShotDelay = 4;
constexpr int SpaceshipShotFactor = 1;
constexpr int SpaceshipFlashDelay = 4;
constexpr int SpaceshipLoadShotFactor = 4;
constexpr int SpaceshipMaxLoadShot = 5;

constexpr int SpaceshipLivesInitial = 3;
constexpr int SpaceshipPowerInitial = 100;
constexpr int SpaceshipMagicInitial = 3;

constexpr int SpaceshipInvincibleTime = 240;
constexpr int SpaceshipBlinkTime = 3;

constexpr int SpaceshipMagicDelay = 240;

// texture coords of ship in motion
constexpr int mov[5][4] = {{101, 3, 32, 14},
                           {134, 3, 32, 13},
                           {167, 3, 32, 12},
                           {200, 3, 32, 13},
                           {233, 2, 32, 14}};
constexpr int movMid = 10;

// texture coords of ship exploding
constexpr int explo[8][4] = {{1, 343, 32, 28}, {35, 343, 32, 28}, {73, 348, 21, 18},
                             {106, 347, 22, 20}, {138, 346, 24, 22}, {170, 345, 26, 24},
                             {202, 344, 28, 26}, {233, 343, 32, 28}};
constexpr int exploMid = 356;

constexpr int shot[8][4] = {{2, 51, 31, 32}, {35, 52, 28, 28}, {68, 54, 29, 28},
                            {103, 55, 25, 25}, {135, 56, 29, 22}, {168, 58, 26, 18},
                            {200, 59, 23, 15}, {233, 60, 20, 12}};
constexpr int disMid = 66;

constexpr int flash[2][4] = {{215, 85, 14, 12}, {233, 87, 11, 9}};
constexpr int flashMid = 91;
}

Spaceship::Spaceship(Engine *engine) : Entity(engine) {
  m_texSpaceship = engine->loadTexture("resources/images/r-typesheet1.png");

  m_pos = {GAME_WIDTH >> 1, GAME_HEIGHT >> 1};
  m_xF = static_cast<float>(m_pos.x);
  m_yF = static_cast<float>(m_pos.y);

  m_lives = SpaceshipLivesInitial;
  m_magic = SpaceshipMagicInitial;

  reset();
}

Spaceship::~Spaceship() = default;

int Spaceship::getLives() const { return m_lives; }

int Spaceship::getMagic() const { return m_magic; }

bool Spaceship::canShootMagic() const {
  return m_magic && m_aliveTime - m_lastMagic >= SpaceshipMagicDelay;
}

int Spaceship::loadShot() const { return m_shotLoad; }

void Spaceship::moveUp() {
  if (m_state == SpaceshipState::Alive) {

    // change coords
    m_yF -= m_pixelsMove;

    if (m_seq == SpaceshipUp) {
      if (m_delay) {
        --m_delay;
      } else {
        m_seq = SpaceshipUpper;
      }
    } else if (m_seq == SpaceshipSide) {
      m_seq = SpaceshipUp;
      m_delay = SpaceshipMoveDelay;
    } else if (m_seq <= SpaceshipDown) {
      m_seq = SpaceshipSide;
    }
  }
}

void Spaceship::moveDown() {
  if (m_state == SpaceshipState::Alive) {

    m_yF += m_pixelsMove;

    if (m_seq == SpaceshipDown) {
      if (m_delay) {
        --m_delay;
      } else {
        m_seq = SpaceshipMoreDown;
      }
    } else if (m_seq == SpaceshipSide) {
      m_seq = SpaceshipDown;
      m_delay = SpaceshipMoveDelay;
    } else if (m_seq >= SpaceshipUp) {
      m_seq = SpaceshipSide;
    }
  }
}

void Spaceship::still() {
  if (m_state == SpaceshipState::Alive) {
    m_seq = SpaceshipSide;
  }
}

void Spaceship::moveRight() { m_xF += m_pixelsMove; }

void Spaceship::moveLeft() { m_xF -= m_pixelsMove; }

void Spaceship::shoot() {
  if (m_pulsedShot) {
    ++m_shotLoad;
  } else {
    m_pulsedShot = true;
    m_shotLoad = 0;
  }
}

void Spaceship::noShoot() {
  // shoot when player releases key
  if (m_pulsedShot) {
    m_pulsedShot = false;

    if (!m_engine->level()) {
      m_shotLoad = 0;
      return;
    }

    // do not allow to shoot too fast
    int64_t interval = m_aliveTime - m_lastShot;
    if (interval < m_shotDelay)
      return;
    m_lastShot = m_aliveTime;

    // the charge time determines how much the projectile will come out
    int xShot = static_cast<int>(m_xF + (mov[m_seq][2] >> 1));
    int yShot = static_cast<int>(m_yF);

    // put the new shot in the level
    int size = std::max(0, m_shotLoad / SpaceshipShotDelay);
    size = std::min(SpaceshipMaxLoadShot, size / SpaceshipLoadShotFactor);
    auto *shot = new SpaceshipShot(m_engine, {xShot, yShot}, SpaceshipShotType::Normal, size);
    m_engine->level()->addShot(shot);

  }
  m_shotLoad = 0;
}

void Spaceship::shootMagic() {
  if (!m_engine->level() || !canShootMagic())
    return;

  m_lastMagic = m_aliveTime;
  --m_magic;
  m_engine->level()->shootMagic();
}

void Spaceship::reset() {
  m_aliveTime = 0;
  m_lastShot = -SpaceshipShotDelay;
  m_lastMagic = -SpaceshipMagicDelay;
  m_power = SpaceshipPowerInitial;
  m_pulsedShot = false;

  m_pixelsMove = SpaceshipMovePixels;

  m_state = SpaceshipState::Alive;
  m_seq = SpaceshipSide;
  m_delay = 0;

  m_shotDelay = SpaceshipShotDelay;

  auto *level = m_engine->level();
  if (level) {
    spawn(level->getRespawnPosition());
  }

  m_lastPos = {int(m_xF), int(m_yF)};
}

void Spaceship::spawn(const glm::ivec2 &pos) {
  m_aliveTime = 0;
  m_lastMagic = -SpaceshipMagicDelay;
  m_lastShot = -SpaceshipShotDelay;
  m_pulsedShot = false;

  m_pos = pos;

  m_xF = static_cast<float>(pos.x);
  m_yF = static_cast<float>(pos.y);
}

void Spaceship::processKeys(const Keys &keys) {
  if (m_state == SpaceshipState::Alive) {
    if (keys[ngf::Scancode::Up])
      moveUp();
    if (keys[ngf::Scancode::Down])
      moveDown();
    if (keys[ngf::Scancode::Left])
      moveLeft();
    if (keys[ngf::Scancode::Right])
      moveRight();
    if (!keys[ngf::Scancode::Up] && !keys[ngf::Scancode::Down])
      still();
    if (keys[ngf::Scancode::Space])
      shoot();
    else
      noShoot();
    if (keys[ngf::Scancode::Z])
      shootMagic();
  }
}

void Spaceship::die() {
  --m_lives;
  m_state = SpaceshipState::Explode;
  m_seq = 0;
  m_delay = SpaceshipExplosionDelay;

  m_shotLoad = 0;
}

ngf::irect Spaceship::getRect() const {
  auto w = mov[m_seq][2] - 4;
  auto h = mov[m_seq][3] - 4;
  auto x = m_pos.x - (w >> 1);
  auto y = m_pos.y - (h >> 1);
  return ngf::irect::fromPositionSize({x, y}, {w, h});
}

void Spaceship::update() {
  if (m_state == SpaceshipState::Alive) {

    // increment the time you have been alive
    ++m_aliveTime;

    auto *level = m_engine->level();
    if (level) {
      // get the hit box
      auto rect = getRect();

      // look for collisions with the stage
      auto collision = level->collideLevel(rect);

      // special treatment for collisions when the ship is invincible
      bool invincible = m_aliveTime < SpaceshipInvincibleTime;
      if (invincible) {
        if (collision.has_value() && collision.value().tile()) {
          // if it is invincible and has collided with a tile, fix its position
          if (collision.value().left() && collision.value().right() &&
              collision.value().up() && collision.value().down()) {
            m_pos = m_lastPos;
            m_xF = static_cast<float>(m_pos.x);
            m_yF = static_cast<float>(m_pos.y);
          } else {
            if (collision.value().up() && !(collision.value().down()))
              m_yF = static_cast<float>(m_yF + collision.value().pos().y - rect.min.y);
            else if (collision.value().down() && !(collision.value().up()))
              m_yF = static_cast<float>(m_yF + collision.value().pos().y - (rect.min.y + rect.getHeight()));
            if (collision.value().left() && !(collision.value().right()))
              m_xF = static_cast<float>(m_xF + collision.value().pos().x - rect.min.x);
            else if (collision.value().right() && !(collision.value().left()))
              m_xF = static_cast<float>(m_xF + collision.value().pos().x - (rect.min.x + rect.getWidth()));
          }

          // ok, now it is possible that it goes out of the limits of the screen
          // if so, push the ship inside
          rect = getRect();
          collision = level->collideLevel(rect);
        }
        if (collision.has_value() && collision.value().screen()) {
          if (collision.value().up())
            m_yF = static_cast<float>(m_yF + collision.value().pos().y - rect.min.y);
          else if (collision.value().down())
            m_yF = static_cast<float>(m_yF + collision.value().pos().y - (rect.min.y + rect.getHeight()));
          if (collision.value().left())
            m_xF = static_cast<float>(m_xF + collision.value().pos().x - rect.min.x);
          else if (collision.value().right())
            m_xF = static_cast<float>(m_xF + collision.value().pos().x - (rect.min.x + rect.getWidth()));
        }
      } else {
        // goes off the screen?
        if (collision.has_value() && collision.value().screen()) {
          if (collision.value().up())
            m_yF = static_cast<float>(m_yF + collision.value().pos().y - rect.min.y);
          else if (collision.value().down())
            m_yF = static_cast<float>(m_yF + collision.value().pos().y - (rect.min.y + rect.getHeight()));
          if (collision.value().left())
            m_xF = static_cast<float>(m_xF + collision.value().pos().x - rect.min.x);
          else if (collision.value().right())
            m_xF = static_cast<float>(m_xF + collision.value().pos().x - (rect.min.x + rect.getWidth()));
          // maybe when pushing the ship, it collides with a tile
          rect = getRect();
          collision = level->collideLevel(rect);
        }
        // hit a tile? let it burst!
        if (collision && collision.value().tile()) {
          m_power = 0;
        }
      }

      // Have you hit a bad shot?
      auto shots = level->shots();
      for (auto shot : shots) {
        if (shot->isBad()) {
          if (shot->collide(rect)) {
            if (!invincible)
              m_power -= shot->getPower();
            shot->die();
          }
        }
      }

      if (m_power <= 0) {
        die();
        return;
      }
    }

    m_pos.x = int(m_xF);
    m_pos.y = int(m_yF);
    m_lastPos = m_pos;

  } else if (m_state == SpaceshipState::Explode) {
    if (m_delay) {
      --m_delay;
    } else {
      ++m_seq;
      m_delay = SpaceshipExplosionDelay;
    }
    if (m_seq == SpaceshipExplosionFrames) {
      if (m_lives)
        reset();
      else {
        m_state = SpaceshipState::Dead;
        m_dead = true;
        m_engine->level()->gameOver();
      }
    }
  }
}

void Spaceship::offset(int x, int y) {
  m_xF += static_cast<float>(x);
  m_yF += static_cast<float>(y);

  m_pos.x = int(m_xF);
  m_pos.y = int(m_yF);
}

void Spaceship::draw(ngf::RenderTarget &target, ngf::RenderStates states) const {
  if (m_state == SpaceshipState::Dead)
    return;

  // blinking indicates the ship is invincible
  bool invincible = m_state == SpaceshipState::Alive && m_aliveTime < SpaceshipInvincibleTime;
  if (invincible) {
    int64_t t = m_aliveTime / SpaceshipBlinkTime;
    if (t % 2)
      return;
  }

  int xTexShip, yTexShip, wTexShip, hTexShip;
  int xPixShip, yPixShip, yPixOffset, wPixShip;

  if (m_state == SpaceshipState::Alive) {
    xTexShip = mov[m_seq][0];
    yTexShip = mov[m_seq][1];
    wTexShip = mov[m_seq][2];
    hTexShip = mov[m_seq][3];
    wPixShip = mov[m_seq][2];
    xPixShip = m_pos.x - (wPixShip >> 1);
    yPixOffset = movMid - mov[m_seq][1];
    yPixShip = (m_pos.y - yPixOffset);
  } else {
    xTexShip = explo[m_seq][0];
    yTexShip = explo[m_seq][1];
    wTexShip = explo[m_seq][2];
    hTexShip = explo[m_seq][3];
    wPixShip = explo[m_seq][2];
    xPixShip = m_pos.x - (wPixShip >> 1);
    yPixOffset = exploMid - explo[m_seq][1];
    yPixShip = (m_pos.y - yPixOffset);
  }

  ngf::Sprite s;
  s.setTexture(*m_texSpaceship);
  s.setTextureRect(
      ngf::irect::fromPositionSize({xTexShip, yTexShip}, {wTexShip, hTexShip}));
  s.getTransform().setPosition({xPixShip, yPixShip});
  s.draw(target, states);

  if (m_state == SpaceshipState::Alive) {
    if (m_shotLoad >= SpaceshipShotFactor * SpaceshipShotDelay) {
      // draw shot load
      int xTexShot, yTexShot, hTexShot;
      int xPixShot, yPixShot, yPixOffsetShot, wPixShot;

      int xShot = m_pos.x + (wPixShip >> 1);
      int yShot = m_pos.y;
      int seqShot = (m_shotLoad / SpaceshipShotDelay) % 8;
      xTexShot = shot[seqShot][0];
      yTexShot = shot[seqShot][1];
      hTexShot = shot[seqShot][3];
      wPixShot = shot[seqShot][2];
      xPixShot = xShot;
      yPixOffsetShot = disMid - shot[seqShot][1];
      yPixShot = (yShot - yPixOffsetShot);

      s.setTextureRect(ngf::irect::fromPositionSize({xTexShot, yTexShot},
                                                    {wPixShot, hTexShot}));
      s.getTransform().setPosition({xPixShot, yPixShot});
      s.draw(target, states);
    }

    int interval = int(m_aliveTime - m_lastShot);
    if (interval < 2 * SpaceshipFlashDelay) {
      // draw the flash of the projectile when it comes out
      int xTexFlash, yTexFlash, hTexFlash;
      int xPixFlash, yPixFlash, yPixOffsetFlash, wPixFlash;

      int xFlash = m_pos.x + (wPixShip >> 1);
      int yFlash = m_pos.y;
      int seqFlash = interval / SpaceshipFlashDelay;
      xTexFlash = flash[seqFlash][0];
      yTexFlash = flash[seqFlash][1];
      hTexFlash = flash[seqFlash][3];
      wPixFlash = flash[seqFlash][2];
      xPixFlash = xFlash;
      yPixOffsetFlash = flashMid - flash[seqFlash][1];
      yPixFlash = (yFlash - yPixOffsetFlash);

      s.setTextureRect(ngf::irect::fromPositionSize({xTexFlash, yTexFlash},
                                                    {wPixFlash, hTexFlash}));
      s.getTransform().setPosition({xPixFlash, yPixFlash});
      s.draw(target, states);
    }
  }
}