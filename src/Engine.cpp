#include <entt/entt.hpp>
#include <ngf/Audio/AudioSystem.h>
#include <ngf/Audio/SoundBuffer.h>
#include <ngf/Math/Transform.h>
#include <ECS/ComponentFactory.h>
#include <Engine.h>
#include <Level.h>
#include <System/Log.h>
#include <ECS/Component/Components.h>
#include <ECS/System/AnimationSystem.h>
#include <ECS/System/MotionSystem.h>
#include <ECS/System/CollisionSystem.h>
#include <ECS/System/RenderSystem.h>
#include <Scripting/EntityManager.h>
#include <Scripting/EventManager.h>
#include <Scripting/Bindings/Bindings.h>
#include <Scripting/SoundManager.h>

template<>
struct fmt::formatter<fs::path> {
  template<typename FormatContext>
  auto format(const fs::path &p, FormatContext &ctx) {
    return format_to(
        ctx.out(),
        p.string());
  }
};

Engine::Engine(ngf::AudioSystem &audio) {
  m_entityManager = std::make_unique<EntityManager>(m_reg, m_lua);
  m_eventManager = std::make_unique<EventManager>(m_lua);
  m_soundManager = std::make_unique<SoundManager>(audio);
  m_componentFactory = std::make_unique<ComponentFactory>();

  createVm();
}

Engine::~Engine() = default;

void Engine::createVm() {
  m_lua.open_libraries();
  Bindings::bindAll(m_lua);
  m_componentFactory->registerComponentType<AnimationComponent>("Animation");
  m_componentFactory->registerComponentType<PositionComponent>("Position");
  m_componentFactory->registerComponentType<MotionComponent>("Motion");
  m_componentFactory->registerComponentType<GraphicComponent>("Graphics");
  m_componentFactory->registerComponentType<NameComponent>("Name");
  m_componentFactory->registerComponentType<CollideComponent>("Collide");
}

std::shared_ptr<ngf::Texture> Engine::loadTexture(const fs::path &path) {
  auto it = m_textures.find(path);
  if (it == m_textures.end()) {
    SPDLOG_INFO("Load texture {}", path.string());
    auto texture = std::make_shared<ngf::Texture>(path);
    m_textures.insert({path, texture});
    return texture;
  }
  return it->second;
}

void Engine::onKeyDown(ngf::Scancode code) {
  m_lua["onKeyDown"].call(code);

}
void Engine::onKeyUp(ngf::Scancode code) {
  m_lua["onKeyUp"].call(code);
}

void Engine::startGame() {

  loadLevel();

  auto r = m_lua.script_file("resources/scripts/boot.lua");
  if (!r.valid()) {
    sol::error err = r;
    RTYPE_LOG_ERROR("[lua] failed to load boot.lua:\n {}", err.what());
  }

  m_gameStarted = true;
}

void Engine::loadLevel() {
  m_level = std::make_unique<Level>("resources/levels/rtype.json", "resources/levels/rtype.png");
  m_reg.set<Level *>(m_level.get());
}

void Engine::update() {
  if (!m_gameStarted)
    return;

  Systems::MotionSystem::update(m_reg);
  Systems::CollisionSystem::update(m_reg);
  Systems::AnimationSystem::update(m_reg);

  auto r = m_lua["update"].call();
  if (!r.valid()) {
    sol::error e = r;
    RTYPE_LOG_ERROR("[lua] failed to call update:\n{}", e.what());
  }
  m_soundManager->update();
}

void Engine::draw(ngf::RenderTarget &target) {
  if (m_level)
    m_level->draw(target);

  Systems::RenderSystem::draw(m_reg, target);
}
