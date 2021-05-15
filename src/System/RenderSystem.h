#pragma once

#include <entt/entt.hpp>
#include <ngf/Graphics/RenderStates.h>
#include <ngf/Graphics/RenderTarget.h>

class Engine;
namespace RenderSystem {

void draw(entt::registry &registry, ngf::RenderTarget &target, ngf::RenderStates states);

}

