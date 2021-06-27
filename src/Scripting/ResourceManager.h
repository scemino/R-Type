#pragma once

#include <Graphics/AnimationLoader.h>
#include <Graphics/TilesLoader.h>
#include <Graphics/TextureLoader.h>
#include <System/Cache.h>

struct ResourceManager {
public:
  Cache<AnimationsInfo, AnimationLoader> animationsCache{};
  Cache<TilesInfo, TilesLoader> tilesCache{};
  Cache<ngf::Texture, TextureLoader> textureCache{};
};
