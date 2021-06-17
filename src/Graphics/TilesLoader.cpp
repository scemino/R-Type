#include "TilesLoader.h"
#include <ngf/IO/Json/JsonParser.h>
#include <System/Log.h>
#include <Engine.h>
#include <System/Locator.h>

TilesInfo loadTiles(const std::filesystem::path &path) {
  Engine &engine = locator::engine::ref();
  RTYPE_LOG_INFO("Load tiles {}", path.string());
  const auto jTiles = ngf::Json::load(path);
  auto tileWidth = jTiles["tilewidth"].getInt();
  auto tileHeight = jTiles["tileheight"].getInt();

  auto texturePath = path;
  texturePath.replace_extension(".png");
  auto texture = engine.loadTexture(texturePath);
  const auto size = texture->getSize();
  return TilesInfo{texture,
                   glm::ivec2{tileWidth, tileHeight},
                   glm::ivec2{size.x / tileWidth, size.y / tileHeight}};
}