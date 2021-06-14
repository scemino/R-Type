#pragma once

#include <sol/sol.hpp>
#include <System/Log.h>

class EventManager {
public:
  explicit EventManager(sol::state &lua) : m_lua(lua) {}

  template<typename... Args>
  void publish(Entity& entity, std::string_view type, Args &&... args) {
    auto data = m_lua.create_table_with(std::forward<Args>(args)...);
    auto event = m_lua.create_table_with("type", type, "data", data);
    auto r = m_lua["onEvent"](entity, event);
    if (!r.valid()) {
      sol::error e = r;
      RTYPE_LOG_ERROR("[lua] failed to call onEvent:\n{}", e.what());
    }
  }

private:
  sol::state &m_lua;
};
