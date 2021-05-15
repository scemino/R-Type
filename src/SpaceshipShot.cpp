#include "Engine.h"
#include "Level.h"
#include "Shot.h"
#include "SpaceshipShot.h"
#include <ngf/Graphics/Sprite.h>

namespace {
constexpr int Delay = 6;
constexpr int CircularDelay = 2;
constexpr int ExplosionDelay = 6;

constexpr int shipShot[14][4] = {
    {303, 85, 14, 12}, {288, 87, 11, 9}, {249, 90, 16, 4},
    {249, 90, 16, 4}, {249, 105, 16, 8}, {232, 103, 16, 12},
    {200, 121, 32, 10}, {233, 120, 32, 12}, {168, 137, 48, 12},
    {217, 136, 48, 14}, {136, 154, 64, 14}, {201, 154, 64, 14},
    {104, 171, 80, 14}, {185, 170, 80, 16}};
constexpr int shipShotMid[6] = {91, 108, 125, 142, 160, 177};

constexpr int circularShotMov[24][4] = {
    {54, 552, 12, 36}, {71, 547, 18, 46}, {104, 543, 27, 54},
    {137, 542, 37, 56}, {187, 542, 47, 56}, {235, 542, 55, 56},
    {40, 608, 61, 56}, {105, 608, 64, 56}, {172, 608, 60, 56},
    {235, 608, 64, 56}, {40, 674, 63, 56}, {105, 674, 60, 56},
    {170, 676, 58, 52}, {238, 678, 58, 48}, {40, 745, 64, 44},
    {105, 750, 64, 32}, {37, 470, 64, 32}, {103, 470, 59, 32},
    {169, 470, 59, 32}, {237, 470, 62, 32}, {37, 504, 64, 32},
    {103, 504, 59, 32}, {169, 504, 59, 32}, {237, 504, 62, 32}};
constexpr int circularShotMid[24] = {569, 569, 569, 569, 569, 569, 635, 635,
                                     635, 635, 701, 701, 701, 701, 765, 765,
                                     485, 485, 485, 485, 519, 519, 519, 519};
constexpr int circularShotAdvance[24] = {0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5,
                                         5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
}

SpaceshipShot::SpaceshipShot(Engine *engine, glm::ivec2 pos, SpaceshipShotType type, int size)
    : Shot(engine, pos), m_size(size), m_type(type) {
  m_texNave1 = engine->loadTexture("resources/images/r-typesheet1.png");
  m_texNave2 = engine->loadTexture("resources/images/r-typesheet2.png");

  m_damage = 20 + m_size * 100;
  m_isBad = false;

  m_pixelsAdvance = shipShot[0][2] - m_size - (m_size >> 1);

  m_delay = !m_size && m_type == SpaceshipShotType::Circular ? CircularDelay : Delay;
}

SpaceshipShot::~SpaceshipShot() = default;

ngf::irect SpaceshipShot::getRect() const {
  if (m_size || m_type == SpaceshipShotType::Normal) {
    int numSeq = 2 + (2 * m_size);
    return ngf::irect::fromPositionSize(m_pos, {shipShot[numSeq][2], shipShot[numSeq][3]});
  }
  return ngf::irect::fromPositionSize({m_pos.x, m_pos.y - (circularShotMov[m_seq][3] >> 1)},
                                      {circularShotMov[m_seq][2], circularShotMov[m_seq][3]});
}

bool SpaceshipShot::collide(const ngf::irect &rect) const {
  if (m_state != ShotState::Alive)
    return false;

  auto myRect = getRect();
  return myRect.intersects(rect);
}

void SpaceshipShot::explode() {
  m_state = ShotState::Explosion;
  m_seq = 0;
  m_delay = ExplosionDelay;
}

void SpaceshipShot::die() {
  if (!m_size && m_type == SpaceshipShotType::Normal) {
    m_state = ShotState::Explosion;
    m_seq = 0;
    m_delay = ExplosionDelay;
  }
}

void SpaceshipShot::update() {
  if (m_state == ShotState::Alive) {
    auto *level = m_engine->level();

    auto circular = !m_size && m_type == SpaceshipShotType::Circular;
    if (circular)
      m_pos.x += circularShotAdvance[m_seq];
    else
      m_pos.x += m_pixelsAdvance;

    // if left off stage, kill him immediately
    if (m_pos.x > GAME_WIDTH + level->getScrollPosition()) {
      // kill shot when off screen
      m_dead = true;
      return;
    }

    // shots have a lower hit box only for collisions with the
    // stage
    auto rect = getRect();
    auto h = std::min(rect.getHeight(), 4);
    auto y = rect.min.y + (rect.getHeight() >> 1) - (h >> 1);
    rect = ngf::irect::fromPositionSize({rect.min.x, y}, {rect.getWidth(), h});

    if (level->collide(rect)) {
      // Although they can pass through several enemies at once,
      // big shots die against the stage
      explode();
      return;
    }

    if (m_delay)
      --m_delay;
    else {
      if (m_size || m_type == SpaceshipShotType::Normal) {
        m_seq = !m_seq;
        m_delay = Delay;
      } else {
        ++m_seq;
        if (m_seq == 24)
          m_seq = 16;
        m_delay = CircularDelay;
      }
    }
  } else if (m_state == ShotState::Explosion) {
    if (m_delay)
      --m_delay;
    else {
      if (m_seq) {
        m_state = ShotState::Dead;
        m_dead = true;
      } else {
        m_seq = !m_seq;
        m_delay = ExplosionDelay;
      }
    }
  }
}
void SpaceshipShot::draw(ngf::RenderTarget &target, ngf::RenderStates states) const {
  if (m_state == ShotState::Dead)
    return;

  // the circular shot has a special treatment
  bool circular = !m_size && m_type == SpaceshipShotType::Circular;

  int xTexShot, yTexShot, hTexShot;
  int xPixShot, yPixShot, yPixOffsetShot, wPixShot;

  ngf::Sprite s;

  if (m_state == ShotState::Alive) {
    s.setTexture(circular ? *m_texNave2 : *m_texNave1);

    // these parameters depend on the type of shot
    int seqShot = circular ? m_seq : 2 + (2 * m_size) + m_seq;
    const auto *shot = circular ? circularShotMov[seqShot] : shipShot[seqShot];
    int mid = circular ? circularShotMid[m_seq] : shipShotMid[m_size];

    xTexShot = shot[0];
    yTexShot = shot[1];
    hTexShot = shot[3];
    wPixShot = shot[2];
    xPixShot = m_pos.x;
    yPixOffsetShot = mid - shot[1];
    yPixShot = (m_pos.y - yPixOffsetShot);
  } else if (m_state == ShotState::Explosion) {
    s.setTexture(*m_texNave1);

    xTexShot = shipShot[m_seq][0];
    yTexShot = shipShot[m_seq][1];
    hTexShot = shipShot[m_seq][3];
    wPixShot = shipShot[m_seq][2];

    if (circular)
      xPixShot = m_pos.x + circularShotMov[m_seq][2] * 4 - shipShot[m_seq][2];
    else
      xPixShot = m_pos.x + shipShot[2 + (2 * m_size)][2] - shipShot[m_seq][2];

    yPixOffsetShot = shipShotMid[0] - shipShot[m_seq][1];
    yPixShot = (m_pos.y - yPixOffsetShot);
  }

  s.setTextureRect(
      ngf::irect::fromPositionSize({xTexShot, yTexShot}, {wPixShot, hTexShot}));
  s.getTransform().setPosition({xPixShot, yPixShot});
  s.draw(target, states);
}