cmake_minimum_required(VERSION 3.22)

project(MY_HUNTER VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
endif()

# include(FetchContent)
# FetchContent_Declare(
#   raylib
#   GIT_REPOSITORY https://github.com/raysan5/raylib.git
#   GIT_TAG 4.5.0
# )
# FetchContent_MakeAvailable(raylib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")

add_executable(
  my_hunter
  ../GameEngine/src/Entity.cpp
  ../GameEngine/src/Registry.cpp
  ../GameEngine/src/utils/AssetManager.cpp
  ../GameEngine/src/utils/Vec2.cpp
  ../GameEngine/src/utils/Math.cpp
  ../GameEngine/src/utils/JsonHandler.cpp
  ../GameEngine/src/system/TransformSystem.cpp
  ../GameEngine/src/system/InputSystem.cpp
  ../GameEngine/src/system/DrawSystem.cpp
  ../GameEngine/src/system/CollisionSystem.cpp
  ../GameEngine/src/system/AnimationSystem.cpp
  ../GameEngine/src/system/ClockSystem.cpp
  ../GameEngine/src/system/NamedFramesSystem.cpp
  ../GameEngine/src/system/HealthSystem.cpp
  ../GameEngine/src/system/SpawnerSystem.cpp
  ../GameEngine/src/system/MouseSystem.cpp
  ../GameEngine/src/system/ButtonSystem.cpp
  ../GameEngine/src/system/MusicSystem.cpp
  ../GameEngine/src/wrapper/RaylibWrapper.cpp
  Src/main.cpp
  Src/SceneLoader.cpp
)

target_include_directories(my_hunter PRIVATE
  ../GameEngine/include
  ../GameEngine/include/component
  ../GameEngine/include/component/rtype-specific
  ../GameEngine/include/system
  ../GameEngine/include/system/rtype-specific
  ../GameEngine/include/utils
  ../GameEngine/include/wrapper
  Includes
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(my_hunter PRIVATE -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(my_hunter PRIVATE -O3 -flto)
endif()

target_compile_options(my_hunter PRIVATE -Wall -Wextra)

target_link_libraries(my_hunter PRIVATE raylib)
