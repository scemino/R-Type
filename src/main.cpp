#include <cstdlib>
#include <ngf/Application.h>
#include <Engine.h>
#include <System/Locator.h>
#include <ECS/Component/Components.h>
#include <Debug/DebugTools.h>
#include <spdlog/spdlog.h>

namespace {
constexpr const char *GameName = "R-Type";
}

class RTypeApplication final : public ngf::Application {
private:
  void onInit() final {
    SPDLOG_INFO("Init {} game", GameName);
    m_window.init({"R-Type", glm::ivec2{640, 480}, true, false, true});
    m_window.setVerticalSyncEnabled();
    restartGame();
  }

  void onQuit() final {
    SPDLOG_INFO("Quit {} game", GameName);
    ngf::Application::onQuit();
  }

  void onRender(ngf::RenderTarget &target) final {
    target.setView(ngf::View(ngf::frect::fromMinMax({0, 0}, {GameWidth, GameHeight})));
    target.clear();
    locator::engine::ref().draw(target);
    Application::onRender(target);
  }

  void onImGuiRender() final {
    auto &registry = locator::engine::ref().registry();
    DebugTools::show(registry);
  }

  void onEvent(ngf::Event &event) final {
    switch (event.type) {
    case ngf::EventType::KeyPressed: {
      locator::engine::ref().onKeyDown(event.key.scancode);
    }
      break;
    case ngf::EventType::KeyReleased: {
      if (event.key.scancode == ngf::Scancode::F5) {
        restartGame();
      }
      locator::engine::ref().onKeyUp(event.key.scancode);
    }
      break;
    default:break;
    }
  }

  void restartGame() {
    SPDLOG_INFO("Start {} game", GameName);
    locator::engine::reset();
    locator::engine::set<Engine>(m_audioSystem);
    locator::engine::ref().startGame();
  }

  void onUpdate(const ngf::TimeSpan &elapsed) final {
    locator::engine::ref().update();
  }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  RTypeApplication app{};
  app.run();
  return EXIT_SUCCESS;
}