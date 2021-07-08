#pragma once

#include <sol/sol.hpp>
#include <System/Log.h>

class EventManager {
public:
  explicit EventManager(sol::state &lua) : m_lua(lua) {}

  template<typename... Args>
  void publish(const sol::table &entity, std::string_view type, Args &&... args) {
    auto data = m_lua.create_table_with(std::forward<Args>(args)...);
    auto event = m_lua.create_table_with("type", type, "data", data);
    m_events.push_back(std::make_tuple(entity, event));
  }

  void sendAll() {
    for (const auto&[entity, event] : m_events) {
      auto r = m_lua["onEvent"](entity, event);
      if (!r.valid()) {
        sol::error e = r;
        RTYPE_LOG_ERROR("[lua] failed to call onEvent:\n{}", e.what());
      }
    }
    m_events.clear();
  }

private:
  sol::state &m_lua;
  std::vector<std::tuple<sol::table, sol::table>> m_events;
};
