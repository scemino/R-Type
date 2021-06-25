#include <glm/vec2.hpp>
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
                              sol::meta_function::multiplication, std::multiplies<glm::vec2>(),
                              sol::meta_function::to_string,
                              [](const glm::vec2 &v1) -> std::string {
                                return "x: " + std::to_string(v1.x) + ", y: " + std::to_string(v1.y);
                              }
  );
}
}