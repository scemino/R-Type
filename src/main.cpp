#include <cstdlib>
#include <ngf/Application.h>
#include <Debug/DebugTools.h>
#include <Engine.h>
#include <System/Locator.h>
#include <System/Log.h>
#include <Replay.h>

namespace {
constexpr const char *GameName = "R-Type";
}

class RTypeApplication final : public ngf::Application {
private:
  void onInit() final {
    RTYPE_LOG_INFO("Init {} game", GameName);
    m_window.init({"R-Type", glm::ivec2{640, 480}, true, false, true});
    m_window.setVerticalSyncEnabled();
    restartGame();
  }

  void onQuit() final {
    RTYPE_LOG_INFO("Quit {} game", GameName);
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
      m_recorder.onKeyDown(event.key.scancode);
      locator::engine::ref().onKeyDown(event.key.scancode);
    }
      break;
    case ngf::EventType::KeyReleased: {
      if (event.key.scancode == ngf::Scancode::F5) {
        restartGame();
      } else if (event.key.scancode == ngf::Scancode::F6) {
        toggleRecord();
      } else if (event.key.scancode == ngf::Scancode::F7) {
        togglePlay();
      }
      m_recorder.onKeyUp(event.key.scancode);
      locator::engine::ref().onKeyUp(event.key.scancode);
    }
      break;
    default:break;
    }
  }

  void toggleRecord() {
    if (!m_recorder.isRecording()) {
      RTYPE_LOG_INFO("Start recording...");
      restartGame();
      m_recorder.start();
    } else {
      RTYPE_LOG_INFO("Stop recording and save");
      m_recorder.stop();
      ReplaySerializer serializer;
      serializer.save(m_recorder.getReplay(), "replay.txt");
    }
  }

  void togglePlay() {
    if (!m_recordPlayer.isPlaying()) {
      RTYPE_LOG_INFO("Start playing...");
      ReplaySerializer serializer;
      auto replay = serializer.load("replay.txt");
      m_recordPlayer.setReplay(replay);
      restartGame();
      m_recordPlayer.start();
    } else {
      m_recordPlayer.stop();
    }
  }

  void restartGame() {
    m_updates = 0;
    RTYPE_LOG_INFO("Start {} game", GameName);
    locator::engine::reset();
    locator::engine::set<Engine>(m_audioSystem);
    locator::engine::ref().startGame();
    m_recordPlayer.setKeyDownHandler([](auto code) { locator::engine::ref().onKeyDown(code); });
    m_recordPlayer.setKeyUpHandler([](auto code) { locator::engine::ref().onKeyUp(code); });
  }

  void onUpdate(const ngf::TimeSpan &elapsed) final {
    m_recordPlayer.update(m_updates);
    locator::engine::ref().update();
    m_recorder.setFrames(m_updates);
    ++m_updates;
  }

private:
  ReplayRecorder m_recorder;
  RecordPlayer m_recordPlayer;
  std::uint32_t m_updates{0};
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  RTypeApplication app{};
  app.run();
  return EXIT_SUCCESS;
}