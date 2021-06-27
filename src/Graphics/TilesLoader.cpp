#include "TilesLoader.h"
#include <ngf/IO/Json/JsonParser.h>
#include <System/Log.h>
#include <Engine.h>
#include <System/Locator.h>
#include <Scripting/ResourceManager.h>
#include <utility>

TilesInfo::TilesInfo(std::shared_ptr<ngf::Texture> tex, const glm::ivec2& ts, const glm::ivec2& nt)
    : texture(std::move(tex)), tileSize(ts), numTiles(nt) {

}

std::shared_ptr<TilesInfo> TilesLoader::load(const std::string &p) const {
  std::filesystem::path path{p};
  Engine &engine = locator::engine::ref();
  RTYPE_LOG_INFO("Load tiles {}", path.string());
  const auto jTiles = ngf::Json::load(path);
  auto tileWidth = jTiles["tilewidth"].getInt();
  auto tileHeight = jTiles["tileheight"].getInt();

  auto texturePath = path;
  texturePath.replace_extension(".png");
  auto texture = engine.resourceManager().textureCache.load(texturePath);
  const auto size = texture->getSize();
  return std::make_shared<TilesInfo>(texture,
                                     glm::ivec2{tileWidth, tileHeight},
                                     glm::ivec2{size.x / tileWidth, size.y / tileHeight});
}