#include <ngf/IO/Json/JsonParser.h>
#include "AnimationLoader.h"

std::unordered_map<std::string, Animation> loadAnimations(const std::filesystem::path &path) {
  std::unordered_map<std::string, Animation> anims;
  const auto jAnims = ngf::Json::load(path);
  for (const auto &jAnimItem : jAnims["animations"].items()) {
    const auto &name = jAnimItem.key();
    const auto &jAnim = jAnimItem.value();
    const auto &jFrames = jAnim["frames"];
    const auto &jDelay = jAnim["delay"];
    const auto delay = jDelay.isNull() ? 6 : jDelay.getInt();
    std::vector<AnimationFrame> frames;
    for (const auto &jFrame : jFrames) {
      frames.push_back(AnimationFrame{ngf::irect::fromPositionSize({
                                                                       jFrame[0].getInt(), jFrame[1].getInt()},
                                                                   {jFrame[2].getInt(), jFrame[3].getInt()})});
    }
    anims[name] = Animation{frames, delay};
  }
  return anims;
}