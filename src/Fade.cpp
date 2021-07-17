#include <ngf/Graphics/RectangleShape.h>
#include "Fade.h"
#include <Engine.h>

void Fade::draw(ngf::RenderTarget &target, const ngf::RenderStates& states) {
  ngf::RectangleShape rect(glm::vec2(GameWidth, GameHeight));
  rect.setColor(ngf::Color(0.f, 0.f, 0.f, fade));
  rect.draw(target, states);
}