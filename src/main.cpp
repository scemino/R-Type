#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <cstdlib>
#include <set>
#include <ngf/Application.h>
#include "Engine.h"
#include "Component/Components.h"
#include "Keys.h"
#include "Debug/DebugTools.h"

class RTypeApplication : public ngf::Application {
private:
  void onInit() override {
    m_window.init({"R-Type", glm::ivec2{640, 480}, true, false, true});
    m_window.setVerticalSyncEnabled();
    m_pEngine = std::make_unique<Engine>();
    m_pEngine->startGame();
  }

  void onRender(ngf::RenderTarget &target) override {
    target.setView(ngf::View(ngf::frect::fromMinMax({0, 0}, {384, 272})));
    target.clear();
    m_pEngine->draw(target, {});
    Application::onRender(target);
  }

  void onImGuiRender() override {
    auto &registry = m_pEngine->registry();
    DebugTools::show(registry);
  }

  void onEvent(ngf::Event &event) override {
    switch (event.type) {
    case ngf::EventType::KeyPressed:m_keys.add(event.key.scancode);
      break;
    case ngf::EventType::KeyReleased:m_keys.remove(event.key.scancode);
      break;
    default:break;
    }
    if (m_keys.contains(ngf::Scancode::F5)) {
      m_restart = true;
    } else if (m_restart) {
      m_restart = false;
      m_pEngine.reset();
      m_pEngine = std::make_unique<Engine>();
      m_pEngine->startGame();
    }
  }

  void onUpdate(const ngf::TimeSpan &elapsed) override {
    m_pEngine->processKeys(m_keys);
    m_pEngine->update();
  }

private:
  std::unique_ptr<Engine> m_pEngine;
  bool m_restart{false};
  Keys m_keys;
};

int main(int argc, char **argv) {
  RTypeApplication app{};
  app.run();
  return EXIT_SUCCESS;
}