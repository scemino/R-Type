#include "Engine.h"
#include "Keys.h"
#include "Level.h"
#include "CollisionResult.h"
#include <ngf/Graphics/RectangleShape.h>
#include <ngf/Graphics/Sprite.h>
#include <ngf/IO/Json/JsonParser.h>
#include "Component/Components.h"
#include "System/CollisionSystem.h"
#include "System/RenderSystem.h"

namespace {
constexpr int MapNumTilesWidth = 48;
constexpr int MapNumTilesHeight = 32;

constexpr int HudHeight = 2;
constexpr int HudHpix = 32;

constexpr int LevelFade = 500;
}

Level::Level(Engine *engine, const char *mapPath, const char *texturePath)
: m_engine(engine)
{
  m_tex = engine->loadTexture(texturePath);
  m_tilesWidthTex = m_tex->getSize().x / TileWidth;

  load(mapPath);

  m_positionFinal = m_numTilesWidth * TileWidth - MapNumTilesWidth * TileWidth;

  m_state = LevelState::Start;
  m_maxFade = LevelFade;
}

Level::~Level() = default;

bool Level::load(const char *path) {
  const auto jMap = ngf::Json::load(path);
  const auto jData = jMap["layers"][0]["data"];
  m_numTilesWidth = jMap["width"].getInt();
  m_numTilesHeight = jMap["height"].getInt();

  int mapSize = m_numTilesWidth * m_numTilesHeight;
  m_tilesMap.resize(mapSize);
  int i = 0;
  for (const auto &jTile : jData) {
    m_tilesMap[i++] = jTile.getInt() - 1;
  }
  return true;
}

void Level::applyScroll() {
  // it's the end of the stage: stop scrolling
  if (m_position >= m_positionFinal)
    return;

  auto offset = 1;
  m_position += offset;

  const auto posView = m_engine->registry().view<ShipComponent, PositionComponent>();
  for (const entt::entity e : posView) {
    auto &pc = posView.get<PositionComponent>(e);
    pc.pos.x += static_cast<float>(offset);
  }
}

std::optional<CollisionResult> Level::collideLevel(const ngf::irect &rect) const {
  int x, y;
  int collisionMask = 0;

  int tileUp = rect.min.y / TileHeight;
  int tileDown = (rect.max.y) / TileHeight;
  int tileLeft = rect.min.x / TileWidth;
  int tileRight = (rect.max.x) / TileWidth;

  //--------------------------------------------------------------------------
  // tiles collision
  //--------------------------------------------------------------------------
  if (tileUp < m_numTilesHeight && tileLeft < m_numTilesWidth &&
      tileRight >= 0 && tileDown >= 0) {
    // up collision
    if (tileUp >= 0) {
      int i = tileUp;
      int j = std::max(0, tileLeft);
      int k = std::min(m_numTilesWidth - 1, tileRight);
      while (j <= k) {
        int tile = m_tilesMap[i * m_numTilesWidth + j];
        if (tile != -1) {
          collisionMask |= CollisionMaskUp;
          y = (tileUp + 1) * TileHeight;
        }
        ++j;
      }
    }
    // down collision
    if (tileDown < m_numTilesHeight) {
      int i = tileDown;
      int j = std::max(0, tileLeft);
      int k = std::min(m_numTilesWidth - 1, tileRight);
      while (j <= k) {
        int tile = m_tilesMap[i * m_numTilesWidth + j];
        if (tile != -1) {
          collisionMask |= CollisionMaskDown;
          y = tileDown * TileHeight;
        }
        ++j;
      }
    }
    // left collision
    if (tileLeft >= 0) {
      int i = std::max(0, tileUp);
      int j = tileLeft;
      int k = std::min(m_numTilesHeight - 1, tileDown);
      while (i <= k) {
        int tile = m_tilesMap[i * m_numTilesWidth + j];
        if (tile != -1) {
          collisionMask |= CollisionMaskLeft;
          x = (tileLeft + 1) * TileWidth;
        }
        ++i;
      }
    }
    // collision right
    if (tileRight < m_numTilesWidth) {
      int i = std::max(0, tileUp);
      int j = tileRight;
      int k = std::min(m_numTilesHeight - 1, tileDown);
      while (i <= k) {
        int tile = m_tilesMap[i * m_numTilesWidth + j];
        if (tile != -1) {
          collisionMask |= CollisionMaskRight;
          x = tileRight * TileWidth;
        }
        ++i;
      }
    }

    if (collisionMask)
      return std::make_optional(CollisionResult{static_cast<CollisionMask>(collisionMask), {x, y},
                                                CollisionObject::Tile});
  }

  // see if it is out of the screen
  if (rect.max.y >= GAME_HEIGHT - HudHpix) {
    collisionMask |= CollisionMaskDown;
    y = GAME_HEIGHT - HudHpix;
  }
  if (rect.min.y < 0) {
    collisionMask |= CollisionMaskUp;
    y = 0;
  }
  if (rect.max.x >= m_position + GAME_WIDTH) {
    collisionMask |= CollisionMaskRight;
    x = m_position + GAME_WIDTH;
  }
  if (rect.min.x < m_position) {
    collisionMask |= CollisionMaskLeft;
    x = m_position;
  }

  if (collisionMask)
    return std::make_optional(CollisionResult{static_cast<CollisionMask>(collisionMask), {x, y},
                                              CollisionObject::Screen});

  return std::nullopt;
}

bool Level::collide(const ngf::irect &rect) const {
  int tileUp = std::max(0, rect.min.y / TileHeight);
  int tileDown = std::min(m_numTilesHeight - 1, (rect.max.y) / TileHeight);
  int tileLeft = std::max(0, rect.min.x / TileWidth);
  int tileRight = std::min(m_numTilesWidth - 1, (rect.max.x) / TileWidth);

  if (tileUp >= m_numTilesHeight || tileLeft >= m_numTilesWidth ||
      tileRight < 0 || tileDown < 0)
    return false;

  // up collision
  int i = tileUp;
  int j = tileLeft;
  while (j <= tileRight) {
    int tile = m_tilesMap[i * m_numTilesWidth + j];
    if (tile != -1)
      return true;
    ++j;
  }

  // down collision
  i = tileDown;
  j = tileLeft;
  while (j <= tileRight) {
    int tile = m_tilesMap[i * m_numTilesWidth + j];
    if (tile != -1)
      return true;
    ++j;
  }

  // left collision
  i = tileUp;
  j = tileLeft;
  while (i <= tileDown) {
    int tile = m_tilesMap[i * m_numTilesWidth + j];
    if (tile != -1)
      return true;
    ++i;
  }

  // right collision
  i = tileUp;
  j = tileRight;
  while (i <= tileDown) {
    int tile = m_tilesMap[i * m_numTilesWidth + j];
    if (tile != -1)
      return true;
    ++i;
  }
  return false;
}

ngf::irect Level::getRect() const {
  auto x = m_position;
  auto y = 0;
  auto w = GAME_WIDTH;
  auto h = GAME_HEIGHT - HudHeight;
  return ngf::irect::fromPositionSize({x, y}, {w, h});
}

void Level::update() {
  applyScroll();

  if (m_state == LevelState::Start) {
    // fade in
    ++m_seq;
    if (m_seq == LevelFade)
      m_state = LevelState::Alive;
  } else if (m_state == LevelState::Dead) {
    // fade out
    --m_seq;
  } else if (m_state == LevelState::Abort) {
    --m_seq;
  }
}

void Level::end() {
  if (m_state != LevelState::Abort) {
    m_state = LevelState::Dead;
    m_maxFade = LevelFade;
  }
}

void Level::draw(ngf::RenderTarget &target, ngf::RenderStates states) const {
  // move camera position
  ngf::Transform t;
  t.setPosition({-m_position, 0});
  states.transform = t.getTransform() * states.transform;

  // draw tiles
  int colIni = m_position / TileWidth;
  ngf::Sprite s;
  s.setTexture(*m_tex);
  for (int i = 0; i < MapNumTilesHeight; ++i) {
    int py = i * TileHeight;
    for (int j = 0; j <= MapNumTilesWidth; ++j) {
      int col = j + colIni;
      if (col >= m_numTilesWidth)
        continue;

      int index = i * m_numTilesWidth + col;
      int tile = m_tilesMap[index];
      if (tile == -1)
        continue;

      auto flipY = (tile & 0x40000000) != 0;
      auto flipX = (tile & 0x80000000) != 0;
      tile &= 0x0FFFFFFF;
      int xTile = (tile % m_tilesWidthTex);
      int yTile = (tile / m_tilesWidthTex);
      int px = col * TileWidth;
      s.setTextureRect(ngf::irect::fromPositionSize(
          {xTile * TileWidth, yTile * TileHeight},
          {TileWidth, TileHeight}));
      s.getTransform().setPosition({px, py});
      s.setFlipY(flipY);
      s.setFlipX(flipX);
      s.draw(target, states);
    }
  }

  RenderSystem::draw(m_engine->registry(), target, states);

  if (m_state == LevelState::Start || m_state == LevelState::Dead || m_state == LevelState::Abort) {
    float alpha = (m_maxFade - m_seq) / static_cast<float>(m_maxFade);
    //  fade in/out
    ngf::RectangleShape fadeShape({GAME_WIDTH, GAME_HEIGHT});
    fadeShape.setColor(ngf::Color(0.f, 0.f, 0.f, alpha));
    fadeShape.getTransform().setPosition({m_position, 0});
    fadeShape.draw(target, states);
  }
}
