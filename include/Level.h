#pragma once

#include "Entity.h"
#include "CollisionResult.h"
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

constexpr int LevelDelay = 1;
constexpr int LevelSpeed = 2;

class Engine;
class Keys;
class Shot;
class Spaceship;

class Level : public Entity {
public:
  Level(Engine *engine, Spaceship *spaceship, const char *mapPath, const char *texturePath);
  ~Level() override;

  [[nodiscard]] int getScrollPosition() const;

  [[nodiscard]] const std::vector<Shot *> &shots() const;

  void addShot(Shot *shot);

  [[nodiscard]] virtual glm::ivec2 getRespawnPosition() const { return {0, 0}; }

  // for the level the box is the rectangle that represents the coordinates of the screen
  [[nodiscard]] ngf::irect getRect() const override;

  [[nodiscard]] std::optional<CollisionResult> collideLevel(const ngf::irect &rect) const;
  [[nodiscard]] bool collide(const ngf::irect &rect) const override;

  void update() override;
  void updateKeys(const Keys &keys);
  void draw(ngf::RenderTarget &target, ngf::RenderStates states) const override;

  void gameOver();
  void end();

  void shootMagic();

protected:
  bool load(const char *path);

  virtual int updatePosition() = 0;        // takes care of the scroll

  void applyScroll();
  void updateEntities();        // makes everyone run their update
  void removeDeads();           // handle dead sprites

protected:
  LevelState m_state{LevelState::Start};
  int m_position{0};      // scroll position
  int m_positionFinal{0}; // scroll end

  Spaceship *m_spaceship{nullptr};
  std::vector<Shot *> m_shots;

  int m_numTilesWidth{0};
  int m_numTilesHeight{0};
  int m_tilesWidthTex{0};
  std::shared_ptr<ngf::Texture> m_tex;

  std::vector<int> m_tilesMap;

  int m_maxFade{0};

  int64_t m_timeMagic{0};
  int64_t m_time{0};
  int m_seq{0};
};
