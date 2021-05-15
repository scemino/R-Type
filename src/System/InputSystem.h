#pragma once

#include <entt/entt.hpp>

class Keys;
namespace InputSystem {
  void update(entt::registry &registry, const Keys &keys);
};
