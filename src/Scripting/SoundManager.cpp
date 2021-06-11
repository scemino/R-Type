#include "SoundManager.h"
#include <ngf/Audio/SoundBuffer.h>
#include <ngf/Audio/SoundHandle.h>

void SoundManager::playSound(const std::string& path)
{
  auto buffer = std::make_unique<ngf::SoundBuffer>();
  buffer->loadFromFile(path);
  auto handle = locator::engine::ref().audio().playSound(*buffer);
  m_sounds.push_back(Sound{std::move(buffer), handle});
}

void SoundManager::update() {
  const auto it = std::remove_if(std::begin(m_sounds), std::end(m_sounds), [](const Sound& s){
    return s.handle->get().getStatus() == ngf::AudioChannel::Status::Stopped;
  });
  m_sounds.erase(it, std::end(m_sounds));
}
