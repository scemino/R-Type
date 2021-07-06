#include "SoundManager.h"
#include <ngf/Audio/SoundBuffer.h>

SoundManager::SoundManager(ngf::AudioSystem &audio) : m_audio(audio) {
}

void SoundManager::playSound(const std::string &path, std::optional<int> loopTimes) {
  auto buffer = std::make_unique<ngf::SoundBuffer>();
  buffer->loadFromFile(path);
  const auto loops = loopTimes.value_or(1);
  auto handle = m_audio.playSound(*buffer, loops);
  m_sounds.push_back(Sound{std::move(buffer), handle});
}

void SoundManager::update() {
  const auto it = std::remove_if(std::begin(m_sounds), std::end(m_sounds), [](const Sound &s) {
    return s.handle->get().getStatus() == ngf::AudioChannel::Status::Stopped;
  });
  m_sounds.erase(it, std::end(m_sounds));
}
