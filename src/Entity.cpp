#include "Entity.h"

Entity::Entity(Engine* pEngine) {
  m_engine = pEngine;
}

Entity::Entity(Engine* pEngine, glm::ivec2 pos) {
  m_engine = pEngine;
  setPosition(pos);
}


glm::ivec2 Entity::getPosition() const {
  return m_pos;
}

void Entity::setPosition(glm::ivec2 pos) {
  m_pos = pos;
}

void Entity::offset(int x, int y) {
  m_pos.x += x;
  m_pos.y += y;
}

bool Entity::isDead() const { return m_dead; }
