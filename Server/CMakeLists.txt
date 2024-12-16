cmake_minimum_required(VERSION 3.22)

project(R-TYPE_SERVER VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")
set(BOOST_INCLUDE_DIRS "C:/local/boost_1_86_0")
set(RAYLIB_INCLUDE_DIRS "../GameEngine/")
set(PYTHON_INCLUDE_DIRS "C:/Users/$ENV{USERNAME}/AppData/Local/Programs/Python/Python311/include")

if(WIN32)
  include(FetchContent)
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

find_package(Python 3.11 REQUIRED Interpreter Development)

add_executable(
  r-type_server
  ../GameEngine/src/Entity.cpp
  ../GameEngine/src/Registry.cpp
  ../GameEngine/src/utils/Vec2.cpp
  ../GameEngine/src/utils/Math.cpp
  ../GameEngine/src/utils/JsonHandler.cpp
  ../GameEngine/src/utils/UUIDManager.cpp
  ../GameEngine/src/system/TransformSystem.cpp
  ../GameEngine/src/system/GravitySystem.cpp
  ../GameEngine/src/system/CollisionSystem.cpp
  ../GameEngine/src/system/ClockSystem.cpp
  ../GameEngine/src/system/HealthSystem.cpp
  ../GameEngine/src/system/ScriptSystem.cpp
  ../GameEngine/src/system/SpawnerSystem.cpp
  ../Network/Src/UDPNetwork.cpp
  ../Network/Src/TCPNetwork.cpp
  Src/ServerSceneLoader.cpp
  Src/main.cpp
  Src/Game.cpp
  Src/NetServer.cpp
)

target_include_directories(r-type_server PRIVATE
  ../GameEngine/include
  ../GameEngine/include/component
  ../GameEngine/include/component/rtype-specific
  ../GameEngine/include/system
  ../GameEngine/include/system/rtype-specific
  ../GameEngine/include/utils
  Includes
  ../Network/Includes
  ${BOOST_INCLUDE_DIRS}
  ${ZLIB_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(r-type_server PRIVATE -g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  target_compile_options(r-type_server PRIVATE -O3 -flto)
endif()

target_link_libraries(r-type_server PRIVATE ZLIB::ZLIB ${Python_LIBRARIES})

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
            -I ${CMAKE_SOURCE_DIR}/Includes
            -I ${CMAKE_SOURCE_DIR}/../Network/Includes
            Src
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running Cppcheck analysis..."
    VERBATIM
  )
endif()

if(WIN32)
else()
  add_custom_target(package_zip ALL
          COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/r-type_server-package.zip" --format=zip -- config scripts r-type_server
          COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/r-type_server-package.zip" "${CMAKE_SOURCE_DIR}/r-type_server-package.zip"
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
          DEPENDS r-type_server
          COMMENT "Creating zip archive with executable, and config directories, and moving it to the project root directory..."
  )
  add_dependencies(package_zip r-type_server)
endif()
