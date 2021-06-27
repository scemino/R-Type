#include "TextureLoader.h"
#include <System/Log.h>

std::shared_ptr<ngf::Texture> TextureLoader::load(const std::string &path) const {
  RTYPE_LOG_INFO("Load texture {}", path);
  return std::make_shared<ngf::Texture>(path);
}
