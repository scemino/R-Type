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
  registry.view<TilesComponent, PositionComponent>()
      .each([&](const auto &tc, const auto &pc) {
        if (!tc.tilesInfo.texture)
          return;
        auto pos = pc.getPosition();
        ngf::Sprite s(*tc.tilesInfo.texture);
        for (const auto tile : tc.tiles) {
          auto x = tile % tc.tilesInfo.numTiles.x;
          auto y = tile / tc.tilesInfo.numTiles.x;
          s.setTextureRect(ngf::irect::fromPositionSize(
              {x * tc.tilesInfo.tileSize.x, y * tc.tilesInfo.tileSize.y},
              tc.tilesInfo.tileSize));
          s.getTransform().setPosition(pos);
          s.draw(target, {});
          pos.x += tc.tilesInfo.tileSize.x;
        }
      });
}
}
