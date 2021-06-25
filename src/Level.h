#pragma once

#include <CollisionResult.h>
#include <entt/entt.hpp>
#include <filesystem>
#include <optional>
#include <vector>
#include <glm/vec2.hpp>
#include <ngf/Graphics/RenderStates.h>
#include <ngf/Graphics/RenderTarget.h>
#include <ngf/Graphics/Texture.h>
#include <ngf/Graphics/Rect.h>

class Engine;
class Keys;

namespace fs = std::filesystem;

class Level {
public:
  Level(const fs::path &mapPath, const fs::path &texturePath);
  ~Level();

  // for the level the box is the rectangle that represents the coordinates of the screen
  [[nodiscard]] ngf::irect getRect() const;

  [[nodiscard]] std::optional<CollisionResult> collideLevel(const ngf::irect &rect) const;
  [[nodiscard]] bool collide(const ngf::irect &rect) const;
  [[nodiscard]] int get(const glm::ivec2 &pos) const;

  void setPosition(int pos);
  [[nodiscard]] int getPosition() const { return m_position; }

  void draw(ngf::RenderTarget &target) const;

private:
  void load(const fs::path &path);

private:
  int m_position{0};      // scroll position
  int m_positionFinal{0}; // scroll end

  int m_numTilesWidth{0};
  int m_numTilesHeight{0};
  int m_tilesWidthTex{0};
  std::shared_ptr<ngf::Texture> m_tex;

  std::vector<int> m_tilesMap;
};
