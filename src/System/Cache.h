#pragma once

#include <memory>
#include <string>
#include <unordered_map>

template<typename T, typename TLoader>
class Cache {
public:
  std::shared_ptr<T> load(const std::string &name) {
    auto it = m_cache.find(name);
    if (it == m_cache.end()) {
      auto resource = TLoader{}.load(name);
      m_cache[name] = resource;
      return resource;
    }
    return it->second;
  }

private:
  std::unordered_map<std::string, std::shared_ptr<T>> m_cache;
};