cmake_minimum_required(VERSION 3.22)

project(R-TYPE_CLIENT VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
endif()

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 4.5.0
)
FetchContent_MakeAvailable(raylib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")
set(BOOST_INCLUDE_DIRS "C:/local/boost_1_86_0")

if(WIN32)
  FetchContent_Declare(
    zlib
    URL https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz
  )
  FetchContent_MakeAvailable(zlib)
  add_library(ZLIB::ZLIB UNKNOWN IMPORTED)
  set_target_properties(ZLIB::ZLIB PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/_deps/zlib-build/Release/zlib.lib"
  )
  set(ZLIB_INCLUDE_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/zlib-src
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/zlib-build
  )
  set(ZLIB_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/_deps/zlib-build/Release/zlib.lib")
else()
  find_package(ZLIB REQUIRED)
endif()

add_executable(
  r-type_client
  ../GameEngine/src/Entity.cpp
  ../GameEngine/src/Registry.cpp
  ../GameEngine/src/utils/AssetManager.cpp
  ../GameEngine/src/utils/Vec2.cpp
  ../GameEngine/src/utils/Math.cpp
  ../GameEngine/src/utils/JsonHandler.cpp
  ../GameEngine/src/utils/UUIDManager.cpp
  ../GameEngine/src/system/TransformSystem.cpp
  ../GameEngine/src/system/InputSystem.cpp
  ../GameEngine/src/system/DrawSystem.cpp
  ../GameEngine/src/system/GravitySystem.cpp
  ../GameEngine/src/system/CollisionSystem.cpp
  ../GameEngine/src/system/AnimationSystem.cpp
  ../GameEngine/src/system/ClockSystem.cpp
  ../GameEngine/src/system/NamedFramesSystem.cpp
  ../GameEngine/src/system/HealthSystem.cpp
  ../GameEngine/src/system/MusicSystem.cpp
  ../GameEngine/src/system/CameraSystem.cpp
  ../GameEngine/src/system/StarFieldSystem.cpp
  ../GameEngine/src/system/MouseSystem.cpp
  ../GameEngine/src/system/ButtonSystem.cpp
  ../GameEngine/src/wrapper/RaylibWrapper.cpp
  ../Network/Src/UDPNetwork.cpp
  ../Network/Src/TCPNetwork.cpp
  Src/main.cpp
  Src/Game.cpp
  Src/NetClient.cpp
  Src/ClientSceneLoader.cpp
)

target_include_directories(r-type_client PRIVATE
  ../GameEngine/include
  ../GameEngine/include/component
  ../GameEngine/include/component/rtype-specific
  ../GameEngine/include/system
  ../GameEngine/include/system/rtype-specific
  ../GameEngine/include/utils
  ../GameEngine/include/wrapper
  Includes
  ../Network/Includes
  ${BOOST_INCLUDE_DIRS}
  ${ZLIB_INCLUDE_DIR}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(r-type_client PRIVATE -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(r-type_client PRIVATE -O3 -flto)
endif()

target_link_libraries(r-type_client PRIVATE raylib ${Boost_LIBRARIES} ZLIB::ZLIB)

find_program(CPPCHECK cppcheck)

if(CPPCHECK)
  add_custom_target(
    cppcheck
    COMMAND ${CPPCHECK}
            --enable=all
            --inconclusive
            --std=c++20
            --force
            --quiet
            --suppress=missingIncludeSystem
            --language=c++
            --enable=warning,performance
            --error-exitcode=1
            --template=gcc
            -I ${CMAKE_SOURCE_DIR}/../GameEngine/include
            -I ${CMAKE_SOURCE_DIR}/../GameEngine/include/component
            -I ${CMAKE_SOURCE_DIR}/../GameEngine/include/system
            -I ${CMAKE_SOURCE_DIR}/../GameEngine/include/utils
            -I ${CMAKE_SOURCE_DIR}/../GameEngine/include/wrapper
            -I ${CMAKE_SOURCE_DIR}/Includes
            -I ${CMAKE_SOURCE_DIR}/../Network/Includes
            ${CMAKE_SOURCE_DIR}/../GameEngine/src
            ${CMAKE_SOURCE_DIR}/Src
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running Cppcheck analysis..."
    VERBATIM
  )
endif()

if(WIN32)
else()
  add_custom_target(package_zip ALL
          COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/r-type_client-package.zip" --format=zip -- assets config r-type_client
          COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/r-type_client-package.zip" "${CMAKE_SOURCE_DIR}/r-type_client-package.zip"
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
          DEPENDS r-type_client
          COMMENT "Creating zip archive with executable, assets, and config directories, and moving it to the project root directory..."
  )
  add_dependencies(package_zip r-type_client)
endif()
