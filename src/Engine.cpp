#include <System/InvincibleSystem.h>
#include <System/MotionSystem.h>
#include <System/CollisionSystem.h>
#include <System/StateSystem.h>
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
  InputSystem::update(m_reg, keys);
}

void Engine::startGame() {
  // create a ship and level
  EntityFactory::createPlayer(m_reg);
  loadLevel();
}

void Engine::loadLevel() {
  m_level = std::make_unique<Level>(this, "resources/levels/rtype.json", "resources/levels/rtype.png");
  m_reg.set<Level *>(m_level.get());
}

void Engine::update() {
  if (m_level)
    m_level->update();
  InvincibleSystem::update(m_reg);
  MotionSystem::update(m_reg);
  CollisionSystem::collide(m_reg);
  StateSystem::update(m_reg);
}

void Engine::draw(ngf::RenderTarget &target, ngf::RenderStates states) {
  if (m_level)
    m_level->draw(target, states);
}

