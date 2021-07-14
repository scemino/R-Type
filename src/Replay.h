#pragma once
#include <vector>
#include <filesystem>
#include <sstream>
#include <ngf/System/Scancode.h>

namespace fs = std::filesystem;

struct ReplayEntry {
  std::uint32_t frame{0};
  union {
    struct {
      std::uint8_t b1: 1, b2: 1, up: 1, dn: 1, r: 1, l: 1, pressed: 1;
    } states;
    std::uint8_t state{0};
  };

  [[nodiscard]] std::string toString() const;
};

struct Replay {
  std::vector<ReplayEntry> entries;
};

class ReplayRecorder {
public:
  void start();
  void stop();

  [[nodiscard]] bool isRecording() const;

  void setFrames(std::uint32_t frames);

  void onKeyDown(ngf::Scancode code);
  void onKeyUp(ngf::Scancode code);

  const Replay& getReplay() const;

private:
  std::uint32_t m_frames{0};
  bool m_recording{false};
  Replay m_replay;
};

class ReplaySerializer {
public:
  Replay load(const fs::path &path);
  void save(const Replay &replay, const fs::path &path);
};

class RecordPlayer {
public:
  using KeyHandler = std::function<void(ngf::Scancode code)>;

public:
  void start();
  void stop();

  [[nodiscard]] bool isPlaying() const;

  void setReplay(Replay Replay);
  void update(std::uint32_t currentFrame);

  void setKeyDownHandler(KeyHandler handler);
  void setKeyUpHandler(KeyHandler handler);

private:
  void onEntry(const ReplayEntry &entry);

private:
  bool m_playing{false};
  std::size_t m_currentIndex{0};
  Replay m_replay;
  KeyHandler m_keyDownHandler;
  KeyHandler m_keyUpHandler;
};
