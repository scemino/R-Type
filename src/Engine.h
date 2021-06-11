#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <entt/entity/registry.hpp>
#include <ngf/Graphics/RenderStates.h>
#include <ngf/System/Scancode.h>
#include <sol/sol.hpp>

namespace fs = std::filesystem;

static constexpr const int GameWidth = 384;
static constexpr const int GameHeight = 272;

class ComponentFactory;
class EntityManager;
class EventManager;
class Level;
class SoundManager;

namespace ngf {
class AudioSystem;
class RenderTarget;
class Texture;
}

class Engine {
public:
  explicit Engine(ngf::AudioSystem &audio);
  ~Engine();

  // TODO: move this to a resource manager
  std::shared_ptr<ngf::Texture> loadTexture(const fs::path &path);

  // services
  sol::state &lua() { return m_lua; }
  entt::registry &registry() { return m_reg; }
  EntityManager &entityManager() { return *m_entityManager; }
  EventManager &eventManager() { return *m_eventManager; }
  ComponentFactory &componentFactory() { return *m_componentFactory; }
  ngf::AudioSystem &audio() { return m_audio; }
  SoundManager &soundManager() { return *m_soundManager; }

  void startGame();

  // input management
  void onKeyDown(ngf::Scancode code);
  void onKeyUp(ngf::Scancode code);

  // update & draw
  void update();
  void draw(ngf::RenderTarget &target);

private:
  void createVm();
  void loadLevel();

private:
  std::map<fs::path, std::shared_ptr<ngf::Texture>> m_textures;
  std::unique_ptr<Level> m_level;
  std::unique_ptr<EntityManager> m_entityManager;
  std::unique_ptr<EventManager> m_eventManager;
  std::unique_ptr<ComponentFactory> m_componentFactory;
  std::unique_ptr<SoundManager> m_soundManager;
  entt::registry m_reg;
  sol::state m_lua;
  ngf::AudioSystem &m_audio;
  bool m_gameStarted{false};
};
