#include "System/ExplodeSystem.h"
#include "System/AnimationSystem.h"
#include "System/HealthSystem.h"
#include "System/InitSystem.h"
#include "System/InvincibleSystem.h"
#include "System/MotionSystem.h"
#include "System/CollisionSystem.h"
#include "System/SpawnSystem.h"
#include "Engine.h"
#include "Level.h"
#include "System/InputSystem.h"
#include "EntityFactory.h"

Engine::Engine() {
  m_reg.set<Engine *>(this);
}

Engine::~Engine() = default;

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
  Systems::InputSystem::update(m_reg, keys);
}

void Engine::startGame() {
  // create a ship and level
  EntityFactory::createPlayer(m_reg);
  Systems::InitSystem::update(m_reg);
  loadLevel();
  update();
}

void Engine::loadLevel() {
  m_level = std::make_unique<Level>(this, "resources/levels/rtype.json", "resources/levels/rtype.png");
  m_reg.set<Level *>(m_level.get());
}

void Engine::update() {
  if (m_level)
    m_level->update();
  Systems::InvincibleSystem::update(m_reg);
  Systems::MotionSystem::update(m_reg);
  Systems::CollisionSystem::update(m_reg);
  Systems::HitSystem::update(m_reg);
  Systems::ExplodeSystem::update(m_reg);
  Systems::AnimationSystem::update(m_reg);
  Systems::SpawnSystem::update(m_reg);
}

void Engine::draw(ngf::RenderTarget &target, ngf::RenderStates states) {
  if (m_level)
    m_level->draw(target, states);
}
