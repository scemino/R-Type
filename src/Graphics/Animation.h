#pragma once

#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include <ngf/Graphics/Rect.h>
#include <ngf/Graphics/Texture.h>

struct AnimationFrame {
  ngf::irect rect;
  glm::vec2 offset{0};
};

struct Animation {
  std::vector<AnimationFrame> frames;
  std::shared_ptr<ngf::Texture> texture;
  int frameDelay{0};
};
