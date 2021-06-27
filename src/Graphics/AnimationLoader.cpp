#include "AnimationLoader.h"
#include <ngf/IO/Json/JsonParser.h>
#include <System/Log.h>
#include <Engine.h>
#include <System/Locator.h>
#include <Scripting/ResourceManager.h>
#include <utility>

namespace {
ngf::irect toRect(const ngf::GGPackValue &jRect) {
  return ngf::irect::fromPositionSize({jRect[0].getInt(), jRect[1].getInt()},
                                      {jRect[2].getInt(), jRect[3].getInt()});
}

glm::vec2 toVector(const ngf::GGPackValue &jVector) {
  return {static_cast<float>(jVector[0].getDouble()), static_cast<float>(jVector[1].getDouble())};
}
}

AnimationsInfo::AnimationsInfo(std::unordered_map<std::string, Animation> animations,
                               std::string initialAnim)
    : animations(std::move(animations)), initialAnim(std::move(initialAnim)) {
}

[[nodiscard]] std::shared_ptr<AnimationsInfo> AnimationLoader::load(const std::filesystem::path &path) const {
  Engine &engine = locator::engine::ref();
  RTYPE_LOG_INFO("Load animations {}", path.string());
  std::unordered_map<std::string, Animation> anims;
  const auto jAnims = ngf::Json::load(path);
  const auto &sDefaultTexture = jAnims["texture"].getString();
  for (const auto &jAnimItem : jAnims["animations"].items()) {
    const auto &name = jAnimItem.key();
    const auto &jAnim = jAnimItem.value();
    const auto &jFrames = jAnim["frames"];
    const auto &jDelay = jAnim["delay"];
    const auto &jLoopFrom = jAnim["loopFrom"];
    const auto &jTexture = jAnim["texture"];
    const auto &jOrigins = jAnim["origins"];
    const auto &sTexture = jTexture.isNull() ? sDefaultTexture : jTexture.getString();
    const auto delay = jDelay.isNull() ? 6 : jDelay.getInt();
    const auto loopFrom = jLoopFrom.isNull() ? 0 : jLoopFrom.getInt();
    std::vector<AnimationFrame> frames;
    for (size_t i = 0; i < jFrames.size(); ++i) {
      const auto jFrame = toRect(jFrames[i]);
      const auto jOrigin = jOrigins.isNull() ?
                           glm::vec2(static_cast<float>(jFrame.getWidth()) / 2.f,
                                     static_cast<float>(jFrame.getHeight()) / 2.f) : toVector(jOrigins[i]);
      frames.push_back(AnimationFrame{jFrame, jOrigin});
    }
    auto texture = engine.resourceManager().textureCache.load(sTexture);
    anims[name] = Animation{frames, texture, delay, static_cast<std::size_t>(loopFrom)};
  }
  return std::make_shared<AnimationsInfo>(anims, jAnims["initialAnim"].getString());
}