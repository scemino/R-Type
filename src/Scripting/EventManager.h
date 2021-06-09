#pragma once

#include <sol/sol.hpp>

class EventManager {
public:
  explicit EventManager(sol::state &lua) : m_lua(lua) {}

  template<typename... Args>
  void publish(Entity *pEntity, std::string_view type, Args &&... args) {
    auto data = m_lua.create_table_with(std::forward<Args>(args)...);
    auto event = m_lua.create_table_with("type", type, "data", data);
    auto r = m_lua["onEvent"](pEntity, event);
    if (!r.valid()) {
      sol::error e = r;
      // TODO: create a logging system
      std::cerr << "[lua] failed to call onEvent:\n" << e.what();
    }
  }

private:
  sol::state &m_lua;
};
