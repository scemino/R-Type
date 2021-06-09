#include "Entity.h"

Entity::Entity(entt::registry &registry, entt::entity e) : m_registry(registry), m_entity(e) {
}
