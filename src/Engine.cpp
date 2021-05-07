#include "Engine.h"
#include "Spaceship.h"
#include "Level.h"
#include "Level1.h"

Engine::Engine() = default;
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

Spaceship *Engine::spaceship() const { return m_spaceship.get(); }

Level *Engine::level() const { return m_level.get(); }

void Engine::processKeys(const Keys &keys) {
  if (m_level)
    m_level->updateKeys(keys);
}

void Engine::updateLevel() {
  if (m_level)
    m_level->update();
}

void Engine::advanceLevel() {
  if (m_level) {
    int levelPos = m_level->getScrollPosition();
    m_spaceship->offset(-levelPos, 0);
  }
}

void Engine::startGame() {
  // create a ship and level
  m_spaceship = std::make_unique<Spaceship>(this);

  loadLevel();
}

void Engine::loadLevel() {
  m_level = std::make_unique<Level1>(this, m_spaceship.get(), "resources/levels/rtype.json", "resources/levels/rtype.png");
  auto pos = m_spaceship->getPosition();
  m_spaceship->spawn(pos);
}

void Engine::update() {
  ++m_time;
  updateLevel();
}

void Engine::drawLevel(ngf::RenderTarget &target, ngf::RenderStates states) {
  if (m_level)
    m_level->draw(target, states);
}

void Engine::draw(ngf::RenderTarget &target, ngf::RenderStates states) {
  drawLevel(target, states);
}
