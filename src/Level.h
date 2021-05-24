#pragma once

#include "CollisionResult.h"
#include <entt/entt.hpp>
#include <optional>
#include <vector>
#include <glm/vec2.hpp>
#include <ngf/Graphics/RenderStates.h>
#include <ngf/Graphics/RenderTarget.h>
#include <ngf/Graphics/Texture.h>
#include <ngf/Graphics/Rect.h>

enum class LevelState {
  Start,
  Alive,
  Dead,
  Abort
};

constexpr int TileWidth = 8;
constexpr int TileHeight = 8;

class Engine;
class Keys;

class Level {
public:
  Level(Engine *engine, const char *mapPath, const char *texturePath);
  ~Level();

  // for the level the box is the rectangle that represents the coordinates of the screen
  [[nodiscard]] ngf::irect getRect() const;

  [[nodiscard]] std::optional<CollisionResult> collideLevel(const ngf::irect &rect) const;
  [[nodiscard]] bool collide(const ngf::irect &rect) const;

  void update();
  void draw(ngf::RenderTarget &target, ngf::RenderStates states) const;

  void end();

  [[nodiscard]] int getScrollPosition() const { return m_position; }

private:
  bool load(const char *path);
  void applyScroll();

private:
  LevelState m_state{LevelState::Start};
  int m_position{0};      // scroll position
  int m_positionFinal{0}; // scroll end

  int m_numTilesWidth{0};
  int m_numTilesHeight{0};
  int m_tilesWidthTex{0};
  std::shared_ptr<ngf::Texture> m_tex;

  std::vector<int> m_tilesMap;

  Engine *m_engine{nullptr};
  int m_maxFade{0};
  int m_seq{0};
};
