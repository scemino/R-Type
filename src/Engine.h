#pragma once

#include <sol/sol.hpp>
#include <map>
#include <memory>
#include <string>
#include <entt/entity/registry.hpp>
#include <ngf/Graphics/RenderTarget.h>
#include <ngf/Graphics/RenderStates.h>
#include <ngf/Graphics/Texture.h>
#include <Scripting/EntityManager.h>
#include <Scripting/EventManager.h>

#define GAME_WIDTH  384
#define GAME_HEIGHT 272

class Keys;
class Level;

class Engine {
public:
  Engine();
  ~Engine();

  std::shared_ptr<ngf::Texture> loadTexture(const std::string &path);

  sol::state &lua() { return m_lua; }
  entt::registry &registry() { return m_reg; }
  EntityManager &entityManager() { return *m_entityManager; }
  EventManager &eventManager() { return *m_eventManager; }

  void startGame();

  void processKeys(const Keys &keys);
  void update();
  void draw(ngf::RenderTarget &target, ngf::RenderStates states);

private:
  void createVm();
  void loadLevel();

private:
  std::map<std::string, std::shared_ptr<ngf::Texture>> m_textures;
  std::unique_ptr<Level> m_level;
  std::unique_ptr<EntityManager> m_entityManager;
  std::unique_ptr<EventManager> m_eventManager;
  entt::registry m_reg;
  sol::state m_lua;
};
