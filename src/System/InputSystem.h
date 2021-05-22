#pragma once

#include <entt/entt.hpp>

class Keys;
namespace Systems::InputSystem {
  void update(entt::registry &registry, const Keys &keys);
};
