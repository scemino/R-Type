#include "Engine.h"
#include "Level.h"
#include "Component/Components.h"
#include "RenderSystem.h"
#include <ngf/Graphics/Sprite.h>

namespace {
constexpr int mov[5][4] = {{101, 3, 32, 14},
                           {134, 3, 32, 13},
                           {167, 3, 32, 12},
                           {200, 3, 32, 13},
                           {233, 2, 32, 14}};

constexpr int beamLoad[8][4] = {{2, 51, 31, 32}, {35, 52, 28, 28}, {68, 54, 29, 28},
                                {103, 55, 25, 25}, {135, 56, 29, 22}, {168, 58, 26, 18},
                                {200, 59, 23, 15}, {233, 60, 20, 12}};
constexpr int shotMid = 66;

constexpr int flash[2][4] = {{215, 85, 14, 12}, {233, 87, 11, 9}};

constexpr int flashMid = 91;

constexpr int shipShot[14][4] = {
    {303, 85, 14, 12}, {288, 87, 11, 9}, {249, 90, 16, 4},
    {249, 90, 16, 4}, {249, 105, 16, 8}, {232, 103, 16, 12},
    {200, 121, 32, 10}, {233, 120, 32, 12}, {168, 137, 48, 12},
    {217, 136, 48, 14}, {136, 154, 64, 14}, {201, 154, 64, 14},
    {104, 171, 80, 14}, {185, 170, 80, 16}};
constexpr int shipShotMid[6] = {91, 108, 125, 142, 160, 177};

ngf::irect toRect(const int *frame) {
  return ngf::irect::fromPositionSize({frame[0], frame[1]}, {frame[2], frame[3]});
}

void drawSpaceship(entt::registry &registry, ngf::RenderTarget &target, ngf::RenderStates states) {
  auto engine = registry.ctx<Engine *>();
  const auto view = registry.view<ShipComponent, PositionComponent>();
  for (const entt::entity e : view) {
    const auto &sc = view.get<ShipComponent>(e);

    // blink if invincible
    const auto invincible = registry.try_get<InvincibleComponent>(e);
    if (invincible) {
      auto t = invincible->delay / 3;
      if (t % 2)
        continue;
    }

    if (sc.state == SpaceshipState::Alive) {
      ngf::irect frame;
      const auto &p = view.get<PositionComponent>(e);

      switch (sc.direction) {
      case ShipDirection::Upper:frame = toRect(mov[4]);
        break;
      case ShipDirection::Up:frame = toRect(mov[3]);
        break;
      case ShipDirection::None:frame = toRect(mov[2]);
        break;
      case ShipDirection::Down:frame = toRect(mov[1]);
        break;
      case ShipDirection::MoreDown:frame = toRect(mov[0]);
        break;
      default:assert(false);
      }

      // draw ship
      const auto texture = engine->loadTexture("resources/images/r-typesheet1.png");
      ngf::Sprite s(*texture);s.setTextureRect(frame);
      s.getTransform().setPosition(p.pos + glm::vec2{0, -7});
      s.draw(target, states);

      // draw beam load
      if (sc.power >= 4) {
        const int seqShot = (sc.power / 4) % 8;
        assert(seqShot >= 0);
        assert(seqShot < 8);
        const auto xTexShot = beamLoad[seqShot][0];
        const auto yTexShot = beamLoad[seqShot][1];
        const auto hTexShot = beamLoad[seqShot][3];
        const auto wPixShot = beamLoad[seqShot][2];
        const auto yPixOffsetShot = shotMid - beamLoad[seqShot][1];
        const auto yPixShot = -yPixOffsetShot;

        const auto offset = glm::vec2(frame.getWidth(), yPixShot);
        const auto beamFrame = ngf::irect::fromPositionSize({xTexShot, yTexShot},
                                                            {wPixShot, hTexShot});

        s.setTextureRect(beamFrame);
        s.getTransform().setPosition(p.pos + offset);
        s.draw(target, states);
      }

      // draw shot flash
      const auto interval = sc.aliveTime - sc.lastShotTime;
      if (interval < 2 * 4) {
        const int seqFlash = static_cast<int>(interval) / 4;
        assert(seqFlash >= 0);
        assert(seqFlash < 2);
        const auto xTexFlash = flash[seqFlash][0];
        const auto yTexFlash = flash[seqFlash][1];
        const auto wPixFlash = flash[seqFlash][2];
        const auto hTexFlash = flash[seqFlash][3];
        const auto yPixOffsetFlash = flashMid - flash[seqFlash][1];

        const auto yPixFlash = -yPixOffsetFlash;

        auto flashFrame = ngf::irect::fromPositionSize({xTexFlash, yTexFlash},
                                                       {wPixFlash, hTexFlash});
        s.setTextureRect(flashFrame);
        s.getTransform().setPosition(p.pos + glm::vec2{frame.getWidth(), yPixFlash});
        s.draw(target, states);
      }
    }
  }
}

void drawShot(entt::registry &registry, ngf::RenderTarget &target, ngf::RenderStates states) {
  auto engine = registry.ctx<Engine *>();
  const auto view = registry.view<ShotComponent, PositionComponent>();
  for (const entt::entity e : view) {
    const auto &p = view.get<PositionComponent>(e);
    const auto &sc = view.get<ShotComponent>(e);

    if (sc.state == SpaceshipState::Alive) {
      const auto texture = engine->loadTexture("resources/images/r-typesheet1.png");

      const int seqShot = 2 + (2 * sc.size) + sc.seq;
      const auto *shot = shipShot[seqShot];
      const int mid = shipShotMid[sc.size];

      const auto xTexShot = shot[0];
      const auto yTexShot = shot[1];
      const auto hTexShot = shot[3];
      const auto wPixShot = shot[2];
      const auto xPixShot = p.pos.x;
      const auto yPixOffsetShot = mid - shot[1];
      const auto yPixShot = (p.pos.y - static_cast<float>(yPixOffsetShot));
      const auto frame = ngf::irect::fromPositionSize({xTexShot, yTexShot}, {wPixShot, hTexShot});

      ngf::Sprite s(*texture, frame);
      s.getTransform().setPosition({xPixShot, yPixShot});
      s.draw(target, states);
    }

  }
}
}

namespace RenderSystem {

void draw(entt::registry &registry, ngf::RenderTarget &target, ngf::RenderStates states) {
  drawSpaceship(registry, target, states);
  drawShot(registry, target, states);
}

}
