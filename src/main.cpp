#include <cstdlib>
#include <set>
#include <ngf/Application.h>
#include "Engine.h"
#include "Keys.h"

class RTypeApplication : public ngf::Application {
private:
  void onInit() override {
    m_window.init({"R-Type", glm::ivec2{640, 480}, true, false, true});
    m_pEngine = std::make_unique<Engine>();
    m_pEngine->startGame();
  }

  void onRender(ngf::RenderTarget &target) override {
    target.setView(ngf::View(ngf::frect::fromMinMax({0,0},{384,272})));
    target.clear();
    m_pEngine->draw(target, {});
    Application::onRender(target);
  }

  void onEvent(ngf::Event &event) override {
    switch (event.type) {
    case ngf::EventType::KeyPressed:
      m_keys.add(event.key.scancode);
      break;
    case ngf::EventType::KeyReleased:
      m_keys.remove(event.key.scancode);
      break;
    default:break;
    }
  }

  void onUpdate(const ngf::TimeSpan &elapsed) override {
    m_pEngine->processKeys(m_keys);
    m_pEngine->update();
  }

private:
  std::unique_ptr<Engine> m_pEngine;
  Keys m_keys;
};

int main(int argc, char **argv) {
  RTypeApplication app{};
  app.run();
  return EXIT_SUCCESS;
}