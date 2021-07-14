#include <optional>
#include <fstream>
#include <iostream>
#include "Replay.h"

namespace {
std::optional<ReplayEntry> createEntry(std::uint32_t frames, ngf::Scancode code, bool pressed) {
  ReplayEntry e;
  e.frame = frames;
  e.state = 0;
  e.states.pressed = pressed;
  switch (code) {
  case ngf::Scancode::Z: e.states.b1 = 1;
    return e;
  case ngf::Scancode::X: e.states.b2 = 1;
    return e;
  case ngf::Scancode::Up: e.states.up = 1;
    return e;
  case ngf::Scancode::Down: e.states.dn = 1;
    return e;
  case ngf::Scancode::Left: e.states.l = 1;
    return e;
  case ngf::Scancode::Right: e.states.r = 1;
    return e;
  default: return std::nullopt;
  }
}

ngf::Scancode toScanCode(const ReplayEntry &entry) {
  if (entry.states.up)
    return ngf::Scancode::Up;
  if (entry.states.dn)
    return ngf::Scancode::Down;
  if (entry.states.l)
    return ngf::Scancode::Left;
  if (entry.states.r)
    return ngf::Scancode::Right;
  if (entry.states.b1)
    return ngf::Scancode::Z;
  if (entry.states.b2)
    return ngf::Scancode::X;
  assert(false);
}
}

[[nodiscard]] std::string ReplayEntry::toString() const {
  std::ostringstream os;
  os << "frame#" << frame << (states.pressed ? " onKeyDown(" : " onKeyUp(");
  if (states.b1) {
    os << "button1)";
  } else if (states.b2) {
    os << "button2)";
  } else if (states.up) {
    os << "up)";
  } else if (states.dn) {
    os << "down)";
  } else if (states.r) {
    os << "right)";
  } else if (states.l) {
    os << "left)";
  } else {
    os << "?)";
  }
  return os.str();
}

void ReplayRecorder::start() {
  m_recording = true;
  m_frames = 0;
  m_replay.entries.clear();
}

void ReplayRecorder::stop() { m_recording = false; }

bool ReplayRecorder::isRecording() const { return m_recording; }

void ReplayRecorder::setFrames(std::uint32_t frames) { m_frames = frames; }

void ReplayRecorder::onKeyDown(ngf::Scancode code) {
  if (m_recording) {
    auto entry = createEntry(m_frames, code, true);
    if (entry.has_value()) {
      m_replay.entries.push_back(entry.value());
    }
  }
}

void ReplayRecorder::onKeyUp(ngf::Scancode code) {
  if (m_recording) {
    auto entry = createEntry(m_frames, code, false);
    if (entry.has_value()) {
      m_replay.entries.push_back(entry.value());
    }
  }
}

const Replay &ReplayRecorder::getReplay() const {
  return m_replay;
}

void RecordPlayer::setKeyDownHandler(KeyHandler handler) { m_keyDownHandler = handler; }

void RecordPlayer::setKeyUpHandler(KeyHandler handler) { m_keyUpHandler = handler; }

void RecordPlayer::onEntry(const ReplayEntry &entry) {
  std::cout << "onEntry: " << entry.toString() << '\n';
  if (entry.states.pressed) {
    m_keyDownHandler(toScanCode(entry));
  } else {
    m_keyUpHandler(toScanCode(entry));
  }
}

void RecordPlayer::start() {
  m_playing = true;
  m_currentIndex = 0;
}

void RecordPlayer::stop() { m_playing = false; }

bool RecordPlayer::isPlaying() const { return m_playing; }

void RecordPlayer::update(std::uint32_t currentFrame) {
  if (!m_playing)
    return;
  const auto &entries = m_replay.entries;
  while (m_currentIndex < entries.size()) {
    const auto &entry = entries[m_currentIndex];
    if (entry.frame != currentFrame) {
      break;
    }

    onEntry(entry);
    ++m_currentIndex;
  }

  if (m_currentIndex == entries.size()) {
    m_playing = false;
  }
}

Replay ReplaySerializer::load(const fs::path &path) {
  Replay replay;
  std::ifstream is(path);
  std::string line;
  while (std::getline(is, line)) {
    std::uint32_t f;
    char c;
    sscanf(line.c_str(), "%u:%c", &f, &c);
    ReplayEntry entry;
    entry.frame = f;
    entry.state = static_cast<std::uint8_t>(c);
    std::cout << entry.toString() << '\n';
    replay.entries.push_back(entry);
  }
  return replay;
}

void ReplaySerializer::save(const Replay &replay, const fs::path &path) {
  std::ofstream os(path);
  for (const auto &entry : replay.entries) {
    os << entry.frame << ':' << entry.state << '\n';
  }
}

void RecordPlayer::setReplay(Replay replay) {
  m_replay = std::move(replay);
}
