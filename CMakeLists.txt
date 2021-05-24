cmake_minimum_required(VERSION 3.5)
project(R-Type LANGUAGES C CXX)

add_subdirectory(extlibs/EnggeFramework)

# Headers
include_directories("src")

# Sources
add_executable(R-Type
        src/Level.cpp
        src/Engine.cpp
        src/System/AnimationSystem.cpp
        src/System/CollisionSystem.cpp
        src/System/ExplodeSystem.cpp
        src/System/HealthSystem.cpp
        src/System/RenderSystem.cpp
        src/System/InitSystem.cpp
        src/System/InputSystem.cpp
        src/System/MotionSystem.cpp
        src/System/InvincibleSystem.cpp
        src/EntityFactory.cpp
        src/main.cpp)

target_compile_features(R-Type PRIVATE cxx_std_17)
set_target_properties(R-Type PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(R-Type ngf)
install(TARGETS R-Type DESTINATION "bin")

