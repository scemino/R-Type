#pragma once

#include <filesystem>
#include <memory>
#include <ngf/Graphics/Texture.h>

struct TilesInfo {
  TilesInfo(std::shared_ptr<ngf::Texture> tex, const glm::ivec2& ts, const glm::ivec2& nt);
  std::shared_ptr<ngf::Texture> texture;
  glm::ivec2 tileSize{0};
  glm::ivec2 numTiles{0};
};

struct TilesLoader final {
  [[nodiscard]] std::shared_ptr<TilesInfo> load(const std::string &path) const;
};
