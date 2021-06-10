#include <entt/entt.hpp>
#include <Scripting/Entity.h>
#include <Component/Components.h>
#include <System/AnimationSystem.h>
#include <System/MotionSystem.h>
#include <System/CollisionSystem.h>
#include <System/CameraSystem.h>
#include <Engine.h>
#include <Level.h>
#include <Log.h>
#include <ComponentFactory.h>
#include <Scripting/Bindings/Bindings.h>
#include <ngf/Audio/AudioSystem.h>

Engine::Engine(ngf::AudioSystem &audio) : m_audio(audio) {
  m_entityManager = std::make_unique<EntityManager>(m_reg, m_lua);
  m_eventManager = std::make_unique<EventManager>(m_lua);
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

std::shared_ptr<ngf::Texture> Engine::loadTexture(const std::string &path) {
  auto it = m_textures.find(path);
  if (it == m_textures.end()) {
    auto texture = std::make_shared<ngf::Texture>(path);
    m_textures.insert({path, texture});
    return texture;
  }
  return it->second;
}

void Engine::processKeys(const Keys &keys) {
}

void Engine::onKeyDown(ngf::Scancode code) {
  m_lua["onKeyDown"].call(code);

}
void Engine::onKeyUp(ngf::Scancode code) {
  m_lua["onKeyUp"].call(code);
}

void Engine::startGame() {
  auto e = m_entityManager->createEntity().getId();
  m_reg.emplace<NameComponent>(e, "camera");
  m_reg.emplace<CameraComponent>(e);
  m_reg.emplace<PositionComponent>(e);
  m_reg.emplace<MotionComponent>(e, glm::vec2{1, 0});
  loadLevel();
  update();

  auto r = m_lua.script_file("resources/scripts/boot.lua");
  if (!r.valid()) {
    sol::error err = r;
    RTYPE_LOG_ERROR("[lua] failed to load boot.lua:\n {}", err.what());
  }
}

void Engine::loadLevel() {
  m_level = std::make_unique<Level>("resources/levels/rtype.json", "resources/levels/rtype.png");
  m_reg.set<Level *>(m_level.get());
}

void Engine::update() {
  if (m_level)
    m_level->update();
  Systems::MotionSystem::update(m_reg);
  Systems::CollisionSystem::update(m_reg);
  Systems::AnimationSystem::update(m_reg);
  Systems::CameraSystem::update(m_reg);

  auto r = m_lua["update"].call();
  if (!r.valid()) {
    sol::error e = r;
    RTYPE_LOG_ERROR("[lua] failed to call update:\n{}", e.what());
  }
}

void Engine::draw(ngf::RenderTarget &target, ngf::RenderStates states) {
  if (m_level)
    m_level->draw(target, states);
}
