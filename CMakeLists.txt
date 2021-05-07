cmake_minimum_required(VERSION 3.5)
project(R-Type LANGUAGES CXX)

add_subdirectory(extlibs/EnggeFramework)

# Headers
include_directories(include)

# Sources
add_executable(R-Type
src/SpaceshipShot.cpp
src/Spaceship.cpp src/Level.cpp src/Level1.cpp
src/Engine.cpp src/Entity.cpp
src/main.cpp)

target_compile_features(R-Type PRIVATE cxx_std_17)
set_target_properties(R-Type PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(R-Type ngf)
install(TARGETS R-Type DESTINATION "bin")
