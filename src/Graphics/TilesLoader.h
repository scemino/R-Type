#pragma once

#include <filesystem>
#include <memory>
#include <ngf/Graphics/Texture.h>

struct TilesInfo {
  std::shared_ptr<ngf::Texture> texture;
  glm::ivec2 tileSize{0};
  glm::ivec2 numTiles{0};
};

TilesInfo loadTiles(const std::filesystem::path &path);
