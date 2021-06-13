#include "Entity.h"

Entity::Entity(entt::entity e) : m_entity(e) {
}

void Entity::die() { m_isDead = true; }
bool Entity::isDead() const { return m_isDead; }
