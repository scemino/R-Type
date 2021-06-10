#include <filesystem>
#include "FileSystemBindings.h"

namespace {
std::string getWorkingDirectory()
{
  return std::filesystem::current_path().string();
}
}

namespace Bindings {
void bindFileSystem(sol::state &lua) {
#define ADD_FUNCTION(x) lua.set_function(#x, x)
  ADD_FUNCTION(getWorkingDirectory);
#undef ADD_FUNCTION
}
}