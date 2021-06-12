#pragma once

#include <entt/entt.hpp>
#include <Engine.h>

struct locator {
  using engine = entt::service_locator<Engine>;
};