cmake_minimum_required(VERSION 3.5)
project(R-Type LANGUAGES C CXX)

add_subdirectory(extlibs/EnggeFramework)

# Headers
include_directories("extlibs")
include_directories("extlibs/lua-5.4.1/src")
include_directories("src")

# Sources
add_executable(R-Type
        src/Level.cpp
        src/Debug/DebugTools.cpp
        src/Engine.cpp
        src/AnimationLoader.cpp
        src/Scripting/Entity.cpp
        src/Scripting/EntityManager.cpp
        src/System/AnimationSystem.cpp
        src/System/CameraSystem.cpp
        src/System/CollisionSystem.cpp
        src/System/RenderSystem.cpp
        src/System/InputSystem.cpp
        src/System/MotionSystem.cpp
        src/System/InvincibleSystem.cpp
        src/EntityFactory.cpp
        src/main.cpp)

set(LUA_SOURCE_FILES
        extlibs/lua-5.4.1/src/lapi.c
        extlibs/lua-5.4.1/src/lauxlib.c
        extlibs/lua-5.4.1/src/lbaselib.c
        extlibs/lua-5.4.1/src/lcode.c
        extlibs/lua-5.4.1/src/lcorolib.c
        extlibs/lua-5.4.1/src/lctype.c
        extlibs/lua-5.4.1/src/ldblib.c
        extlibs/lua-5.4.1/src/ldebug.c
        extlibs/lua-5.4.1/src/ldo.c
        extlibs/lua-5.4.1/src/ldump.c
        extlibs/lua-5.4.1/src/lfunc.c
        extlibs/lua-5.4.1/src/lgc.c
        extlibs/lua-5.4.1/src/linit.c
        extlibs/lua-5.4.1/src/liolib.c
        extlibs/lua-5.4.1/src/llex.c
        extlibs/lua-5.4.1/src/lmathlib.c
        extlibs/lua-5.4.1/src/lmem.c
        extlibs/lua-5.4.1/src/loadlib.c
        extlibs/lua-5.4.1/src/lobject.c
        extlibs/lua-5.4.1/src/lopcodes.c
        extlibs/lua-5.4.1/src/loslib.c
        extlibs/lua-5.4.1/src/lparser.c
        extlibs/lua-5.4.1/src/lstate.c
        extlibs/lua-5.4.1/src/lstring.c
        extlibs/lua-5.4.1/src/lstrlib.c
        extlibs/lua-5.4.1/src/ltable.c
        extlibs/lua-5.4.1/src/ltablib.c
        extlibs/lua-5.4.1/src/ltm.c
        extlibs/lua-5.4.1/src/lundump.c
        extlibs/lua-5.4.1/src/lutf8lib.c
        extlibs/lua-5.4.1/src/lvm.c
        extlibs/lua-5.4.1/src/lzio.c)
add_library(lua ${LUA_SOURCE_FILES})

target_compile_features(R-Type PRIVATE cxx_std_17)
set_target_properties(R-Type PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(R-Type ngf lua)
install(TARGETS R-Type DESTINATION "bin")

