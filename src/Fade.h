#pragma once

struct Fade {
  float fade{0.f};

  void draw(ngf::RenderTarget &target);
};
