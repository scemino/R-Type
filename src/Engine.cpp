#include <Scripting/Entity.h>
#include "Component/Components.h"
#include "System/AnimationSystem.h"
#include "System/InvincibleSystem.h"
#include "System/MotionSystem.h"
#include "System/CollisionSystem.h"
#include "Engine.h"
#include "Level.h"
#include "System/InputSystem.h"
#include "EntityFactory.h"

Engine::Engine() {
  m_entityManager = std::make_unique<EntityManager>(m_reg, m_lua);
  m_eventManager = std::make_unique<EventManager>(m_lua);
  m_reg.set<Engine *>(this);
  m_reg.set<sol::state *>(&m_lua);

  m_lua.open_libraries();
  m_lua.new_usertype<Entity>("Entity", sol::call_constructor,
                             sol::factories([&]() { return m_entityManager->createEntity(); }),
                             "emplace", sol::as_function(&Entity::emplace),
                             "remove", [&](Entity *pEntity) { return m_entityManager->removeEntity(pEntity->getId()); },
                             "getId", &Entity::getId,
                             "getPos", &Entity::getPos,
                             "setPos", &Entity::setPos,
                             "setAnim", &Entity::setAnim,
                             "getAnim", &Entity::getAnim);

  m_lua.new_usertype<glm::ivec2>("vec",
                                 sol::call_constructor,
                                 sol::factories([]() {
                                                  return glm::ivec2{};
                                                }, [](const glm::ivec2 &v) {
                                                  return glm::ivec2{v};
                                                },
                                                [](int x, int y) {
                                                  return glm::ivec2{x, y};
                                                }),
                                 "x",
                                 &glm::ivec2::x,
                                 "y",
                                 &glm::ivec2::y,
                                 sol::meta_function::equal_to,
                                 [](const glm::ivec2 &v1, const glm::ivec2 &v2) -> bool { return v1 == v2; },
                                 sol::meta_function::subtraction,
                                 [](const glm::ivec2 &v1, const glm::ivec2 &v2) -> glm::ivec2 { return v1 - v2; },
                                 sol::meta_function::addition,
                                 [](const glm::ivec2 &v1, const glm::ivec2 &v2) -> glm::ivec2 { return v1 + v2; },
                                 sol::meta_function::division,
                                 [](const glm::ivec2 &v1, const glm::ivec2 &v2) -> glm::ivec2 { return v1 / v2; },
                                 sol::meta_function::multiplication,
                                 [](const glm::ivec2 &v1, const glm::ivec2 &v2) -> glm::ivec2 { return v1 * v2; },
                                 sol::meta_function::modulus,
                                 [](const glm::ivec2 &v1, const glm::ivec2 &v2) -> glm::ivec2 { return v1 % v2; },
                                 sol::meta_function::to_string,
                                 [](const glm::ivec2 &v1) -> std::string {
                                   return "x: " + std::to_string(v1.x) + ", y: " + std::to_string(v1.y);
                                 }
  );
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

template<> struct sol::is_container<glm::ivec2> : std::false_type {};

void Engine::startGame() {
  // create a ship and level
  EntityFactory::createPlayer(*m_entityManager);
  loadLevel();
  update();

  m_lua.script_file("resources/scripts/boot.lua");
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
  Systems::AnimationSystem::update(m_reg);

  m_lua["update"].call();
}

void Engine::draw(ngf::RenderTarget &target, ngf::RenderStates states) {
  if (m_level)
    m_level->draw(target, states);
}
