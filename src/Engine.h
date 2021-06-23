#pragma once

#include <cassert>
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
class DebugManager;
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
  EntityManager &entityManager() { assert(m_entityManager); return *m_entityManager; }
  EventManager &eventManager() { assert(m_eventManager); return *m_eventManager; }
  ComponentFactory &componentFactory() { assert(m_componentFactory); return *m_componentFactory; }
  SoundManager &soundManager() { assert(m_soundManager); return *m_soundManager; }
  DebugManager &debugManager() { assert(m_debugManager); return *m_debugManager; }
  Level& level() { assert(m_level); return *m_level; }

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
  std::unique_ptr<DebugManager> m_debugManager;
  entt::registry m_reg;
  sol::state m_lua;
  bool m_gameStarted{false};
};
