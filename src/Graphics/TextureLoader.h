#pragma once

#include <memory>
#include <string>
#include <ngf/Graphics/Texture.h>

struct TextureLoader final {
  [[nodiscard]] std::shared_ptr<ngf::Texture> load(const std::string &path) const;
};
