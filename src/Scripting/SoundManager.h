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
  void playSound(const std::string& path);
  void update();

private:
  std::vector<Sound> m_sounds;
};
