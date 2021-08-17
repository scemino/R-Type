#include <ngf/Graphics/Sprite.h>
#include <ngf/IO/Json/JsonParser.h>
#include <Engine.h>
#include <System/Locator.h>
#include <Level.h>
#include <CollisionResult.h>
#include <ECS/Component/Components.h>
#include <Scripting/ResourceManager.h>

namespace {
constexpr int TileWidth = 8;
constexpr int TileHeight = 8;
constexpr int TileHudHeight = 2;

constexpr int MapNumTilesWidth = GameWidth / TileWidth;
constexpr int MapNumTilesHeight = GameHeight / TileHeight - TileHudHeight;

constexpr int HudHpix = TileHudHeight * TileHeight;
}

Level::Level(const fs::path &mapPath, const fs::path &texturePath) {
  m_tex = locator::engine::ref().resourceManager().textureCache.load(texturePath);
  m_tilesWidthTex = m_tex->getSize().x / TileWidth;

  load(mapPath);

  m_positionFinal = m_numTilesWidth * TileWidth - MapNumTilesWidth * TileWidth;
}

Level::~Level() = default;

void Level::load(const fs::path &path) {
  const auto jMap = ngf::Json::load(path);
  const auto jData = jMap["layers"][0]["data"];
  m_numTilesWidth = jMap["width"].getInt();
  m_numTilesHeight = jMap["height"].getInt();

  // read tiles
  int mapSize = m_numTilesWidth * m_numTilesHeight;
  m_tilesMap.resize(mapSize);
  int i = 0;
  for (const auto &jTile : jData) {
    m_tilesMap[i++] = jTile.getInt() - 1;
  }

  // read objects
  i = 0;
  const auto jObjects = jMap["layers"][1]["objects"];
  m_objects.resize(jObjects.size());
  for (const auto &jObject : jObjects) {
    const glm::vec2 pos{jObject["x"].getDouble(), jObject["y"].getDouble()};
    std::map<std::string, int> properties;
    for (const auto &jObj : jObject["properties"]) {
      properties[jObj["name"].getString()] = jObj["value"].getInt();
    }
    m_objects[i++] = {jObject["name"].getString(), pos, properties};
  }
}

std::optional<CollisionResult> Level::collideLevel(const ngf::irect &rect) const {
  int x, y;
  int collisionMask = 0;

  int tileUp = rect.min.y / TileHeight;
  int tileDown = (rect.max.y) / TileHeight;
  int tileLeft = (m_position + rect.min.x) / TileWidth;
  int tileRight = (m_position + rect.max.x) / TileWidth;

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
  if (rect.max.y >= (GameHeight - HudHpix)) {
    collisionMask |= CollisionMaskDown;
    y = GameHeight - HudHpix;
  }
  if (rect.min.y < 0) {
    collisionMask |= CollisionMaskUp;
    y = 0;
  }
  if (rect.max.x >= GameWidth) {
    collisionMask |= CollisionMaskRight;
    x = GameWidth;
  }
  if (rect.min.x < 0) {
    collisionMask |= CollisionMaskLeft;
    x = 0;
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

[[nodiscard]] int Level::get(const glm::ivec2 &pos) const {
  const auto x = pos.x / TileWidth;
  if (x < 0 || x >= m_numTilesWidth)
    return -1;
  const auto y = pos.y / TileHeight;
  if (y < 0 || y >= m_numTilesHeight)
    return -1;
  const int index = y * m_numTilesWidth + x;
  const int tile = m_tilesMap.at(index);
  return tile;
}

void Level::setPosition(int pos) {
  m_position = std::clamp(pos, 0, m_positionFinal);
}

void Level::draw(ngf::RenderTarget &target, const ngf::RenderStates &st) const {
  // move camera position
  ngf::Transform t;
  t.setPosition({-m_position, 0});
  ngf::RenderStates states = st;
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
}
const std::vector<LevelObject> &Level::getLevelObjects() {
  return m_objects;
}
