#include <ECS/Component/Components.h>
#include <ECS/System/RenderSystem.h>
#include <ngf/Graphics/Sprite.h>

namespace Systems::RenderSystem {

void draw(entt::registry &registry, ngf::RenderTarget &target) {
  registry.view<GraphicComponent, PositionComponent,NameComponent>()
      .each([&](const auto &gc, const auto &pc, const auto& nc) {
        if(!gc.texture)
          return;
        if (!gc.visible)
          return;
        ngf::Sprite s(*gc.texture, gc.frame);
        s.getTransform().setOrigin(gc.origin);
        s.getTransform().setPosition(pc.getPosition());
        s.draw(target, {});
      });
}
}
