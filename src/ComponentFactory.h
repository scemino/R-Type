#pragma once

#include <functional>
#include <optional>
#include <unordered_map>
#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include <Engine.h>
#include <Locator.h>

class ComponentFactory {
private:
  using factory = std::function<void(const entt::entity e, std::optional<sol::table> t)>;

public:
  template<typename T>
  void registerComponentType(const std::string &name) {
    m_factories[name] = [](const entt::entity e, std::optional<sol::table> t) {
      if(t.has_value()) {
        locator::engine::ref().registry().emplace<T>(e, t.value());
      } else {
        locator::engine::ref().registry().emplace<T>(e);
      }
    };
  }

  void emplace(const entt::entity e, const std::string &name, std::optional<sol::table> t) {
    m_factories[name](e, t);
  }

private:
  std::unordered_map<std::string, factory> m_factories;
};