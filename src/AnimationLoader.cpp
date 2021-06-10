#include <ngf/IO/Json/JsonParser.h>
#include "AnimationLoader.h"
#include <Log.h>

AnimationsInfo loadAnimations(Engine &engine, const std::filesystem::path &path) {
  RTYPE_LOG_INFO("Load animations {}", path.string());
  std::unordered_map<std::string, Animation> anims;
  const auto jAnims = ngf::Json::load(path);
  for (const auto &jAnimItem : jAnims["animations"].items()) {
    const auto &name = jAnimItem.key();
    const auto &jAnim = jAnimItem.value();
    const auto &jFrames = jAnim["frames"];
    const auto &jDelay = jAnim["delay"];
    const auto &sTexture = jAnim["texture"].getString();
    const auto delay = jDelay.isNull() ? 6 : jDelay.getInt();
    std::vector<AnimationFrame> frames;
    for (const auto &jFrame : jFrames) {
      frames.push_back(AnimationFrame{
          ngf::irect::fromPositionSize(
              {jFrame[0].getInt(), jFrame[1].getInt()},
              {jFrame[2].getInt(), jFrame[3].getInt()})});
    }
    auto texture = engine.loadTexture(sTexture);
    anims[name] = Animation{frames, texture, delay};
  }
  return AnimationsInfo{anims, jAnims["initialAnim"].getString()};
}