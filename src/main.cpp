#include <cstdlib>
#include <ngf/Application.h>
#include <Engine.h>
#include <Locator.h>
#include <Component/Components.h>
#include <Keys.h>
#include <Debug/DebugTools.h>

class RTypeApplication final : public ngf::Application {
private:
  void onInit() override {
    m_window.init({"R-Type", glm::ivec2{640, 480}, true, false, true});
    m_window.setVerticalSyncEnabled();
    locator::engine::set<Engine>();
    locator::engine::ref().startGame();
  }

  void onRender(ngf::RenderTarget &target) override {
    target.setView(ngf::View(ngf::frect::fromMinMax({0, 0}, {384, 272})));
    target.clear();
    locator::engine::ref().draw(target, {});
    Application::onRender(target);
  }

  void onImGuiRender() override {
    auto &registry = locator::engine::ref().registry();
    DebugTools::show(registry);
  }

  void onEvent(ngf::Event &event) override {
    switch (event.type) {
    case ngf::EventType::KeyPressed:m_keys.add(event.key.scancode);
      locator::engine::ref().onKeyDown(event.key.scancode);
      break;
    case ngf::EventType::KeyReleased:m_keys.remove(event.key.scancode);
      locator::engine::ref().onKeyUp(event.key.scancode);
      break;
    default:break;
    }
    if (m_keys.contains(ngf::Scancode::F5)) {
      m_restart = true;
    } else if (m_restart) {
      m_restart = false;
      locator::engine::reset();
      locator::engine::set<Engine>();
      locator::engine::ref().startGame();
    }
  }

  void onUpdate(const ngf::TimeSpan &elapsed) override {
    locator::engine::ref().processKeys(m_keys);
    locator::engine::ref().update();
  }

private:
  bool m_restart{false};
  Keys m_keys;
};

int main(int argc, char **argv) {
  RTypeApplication app{};
  app.run();
  return EXIT_SUCCESS;
}