#pragma once

#include <entt/entity/registry.hpp>
#include <map>
#include <memory>
#include <string>
#include <ngf/Graphics/RenderTarget.h>
#include <ngf/Graphics/RenderStates.h>
#include <ngf/Graphics/Texture.h>

#define GAME_WIDTH  384
#define GAME_HEIGHT 272

class Entity;
class Keys;
class Level;
class Spaceship;

class Engine {
public:
  Engine();
  ~Engine();

  std::shared_ptr<ngf::Texture> loadTexture(const std::string &path);

  [[nodiscard]] Spaceship *spaceship() const;
  [[nodiscard]] Level *level() const;

  entt::registry &registry() { return m_reg; }

  void advanceLevel();
  void startGame();

  void processKeys(const Keys &keys);
  void update();
  void draw(ngf::RenderTarget &target, ngf::RenderStates states);

private:
  void loadLevel();
  void updateLevel();
  void drawLevel(ngf::RenderTarget &target, ngf::RenderStates states);

private:
  std::map<std::string, std::shared_ptr<ngf::Texture>> m_textures;
  std::unique_ptr<Spaceship> m_spaceship;
  std::unique_ptr<Level> m_level;
  int64_t m_time{0};
  entt::registry m_reg;
};
