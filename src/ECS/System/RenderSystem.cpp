#include <ECS/Component/Components.h>
#include <ECS/System/RenderSystem.h>
#include <ngf/Graphics/Sprite.h>
#include <ngf/Graphics/RectangleShape.h>
#include <ngf/Graphics/CircleShape.h>
#include <Scripting/DebugManager.h>

namespace {
ngf::frect getHitBox(const ngf::frect &rect, const glm::vec2 &pos) {
  ngf::frect r = rect;
  r.min += pos;
  r.max += pos;
  return r;
}
}

namespace Systems::RenderSystem {

void draw(entt::registry &registry, ngf::RenderTarget &target) {
  registry.sort<GraphicComponent>([](const auto &lhs, const auto &rhs) {
    return lhs.zOrder < rhs.zOrder;
  });
  registry.view<GraphicComponent, PositionComponent, NameComponent>()
      .each([&](const auto &gc, const auto &pc, const auto &nc) {
        if (!gc.texture)
          return;
        if (!gc.visible)
          return;
        ngf::Sprite s(*gc.texture, gc.frame);
        s.setFlipX(gc.flipX);
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

  // debug collision hit boxes
  if(!locator::engine::ref().debugManager().showHitboxes) return;

  const auto view = registry.view<CollideComponent, PositionComponent>();
  for (const entt::entity e : view) {
    const auto &[cc, pc] = registry.get<CollideComponent, PositionComponent>(e);
    const auto rect = getHitBox(cc.hitbox, pc.getPosition());

    // draw hit box
    ngf::RectangleShape hb(rect);
    hb.setOutlineThickness(1);
    hb.setColor(ngf::Color{0xff, 0x00, 0x00, 0x70});
    hb.setOutlineColor(ngf::Colors::Red);
    hb.draw(target, {});

    // draw pos
    ngf::CircleShape posShape(2);
    posShape.setColor(ngf::Color{0x00, 0x00, 0xFF, 0xFF});
    posShape.getTransform().setPosition(pc.getPosition());
    posShape.draw(target, {});
  }
}
}
