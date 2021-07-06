#pragma once
#include <memory>
#include <string>
#include <vector>
#include <ngf/Audio/AudioSystem.h>
#include <Engine.h>
#include <System/Locator.h>

namespace ngf {
class SoundBuffer;
class SoundHandle;
}

class SoundManager {
private:
  struct Sound {
    std::unique_ptr<ngf::SoundBuffer> buffer;
    std::shared_ptr<ngf::SoundHandle> handle;
  };

public:
  explicit SoundManager(ngf::AudioSystem &audio);
  void playSound(const std::string& path, std::optional<int> loopTimes);
  void update();

private:
  ngf::AudioSystem &m_audio;
  std::vector<Sound> m_sounds;
};
