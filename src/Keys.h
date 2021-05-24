#pragma once

#include <set>
#include <ngf/System/Scancode.h>

class Keys {
public:
  inline bool contains(ngf::Scancode code) const { return m_keys.find(code) != m_keys.cend(); }
  inline bool operator[](ngf::Scancode code) const { return m_keys.find(code) != m_keys.cend(); }

  inline void add(ngf::Scancode code) { m_keys.insert(code); }
  inline void remove(ngf::Scancode code) { m_keys.erase(code); }

private:
  std::set<ngf::Scancode> m_keys;
};
