#include "Component/Components.h"
#include "RenderSystem.h"
#include <ngf/Graphics/Sprite.h>

namespace Systems::RenderSystem {

void draw(entt::registry &registry, ngf::RenderTarget &target, ngf::RenderStates states) {
  registry.view<GraphicComponent, PositionComponent,NameComponent>()
      .each([&](const auto &gc, const auto &pc, const auto& nc) {
        if(!gc.texture)
          return;
        if (!gc.visible)
          return;
        ngf::Sprite s(*gc.texture, gc.frame);
        s.getTransform().setPosition(pc.pos + gc.offset);
        s.draw(target, states);
      });
}
}
