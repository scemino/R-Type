#include <glm/vec2.hpp>
#include <ngf/Graphics/Rect.h>
#include "MathBindings.h"

namespace Bindings {
void bindMath(sol::state &lua) {
  lua.new_usertype<glm::vec2>("vec",
                              sol::call_constructor,
                              sol::factories([]() {
                                               return glm::vec2{};
                                             }, [](const glm::vec2 &v) {
                                               return glm::vec2{v};
                                             },
                                             [](float x, float y) {
                                               return glm::vec2{x, y};
                                             }),
                              "x", &glm::vec2::x,
                              "y", &glm::vec2::y,
                              sol::meta_function::equal_to, std::equal_to<glm::vec2>(),
                              sol::meta_function::subtraction, std::minus<glm::vec2>(),
                              sol::meta_function::addition, std::plus<glm::vec2>(),
                              sol::meta_function::division, std::divides<glm::vec2>(),
                              sol::meta_function::multiplication,
                              sol::overload([](const glm::vec2 &v, float factor) { return v * factor; },
                                            std::multiplies<glm::vec2>()),
                              sol::meta_function::to_string,
                              [](const glm::vec2 &v1) -> std::string {
                                return "x: " + std::to_string(v1.x) + ", y: " + std::to_string(v1.y);
                              }
  );

  lua.new_usertype<ngf::frect>("rect",
                               sol::call_constructor,
                               sol::factories([]() {
                                                return ngf::frect{};
                                              },
                                              [](float x, float y, float w, float h) {
                                                return ngf::frect::fromPositionSize({x, y}, {w, h});
                                              },
                                              [](const glm::vec2 &pos, const glm::vec2 &size) {
                                                return ngf::frect::fromPositionSize(pos, size);
                                              }),
                               "min", &ngf::frect::min,
                               "max", &ngf::frect::max,
                               sol::meta_function::to_string,
                               [](const ngf::frect &r) -> std::string {
                                 return '(' + std::to_string(r.min.x) + ',' + std::to_string(r.min.y) + ','
                                     + std::to_string(r.max.x) + ',' + std::to_string(r.max.y) + ')';
                               }
  );
}
}