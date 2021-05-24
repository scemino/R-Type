#pragma once

#include <entt/entt.hpp>
#include <glm/vec2.hpp>

namespace EntityFactory {
entt::entity createPlayer(entt::registry &registry);
entt::entity createSpaceshipShot(entt::registry &registry, const glm::vec2& pos, int power);
entt::entity createEnemy1(entt::registry &registry, const glm::vec2& pos);
}
