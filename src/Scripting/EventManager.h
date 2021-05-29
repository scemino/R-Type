#pragma once

#include <sol/sol.hpp>

class EventManager {
public:
  explicit EventManager(sol::state &lua) : m_lua(lua) {}

  template<typename... Args>
  void publish(Entity *pEntity, std::string_view type, Args &&... args) {
    auto data = m_lua.create_table_with(std::forward<Args>(args)...);
    auto event = m_lua.create_table_with("type", type, "data", data);
    m_lua["onEvent"](pEntity, event);
  }

private:
  sol::state &m_lua;
};
