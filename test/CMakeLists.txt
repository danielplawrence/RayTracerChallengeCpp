cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(RayTracerChallengeTests LANGUAGES CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage("gh:onqtam/doctest#2.4.5")
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.0")
CPMAddPackage(
  NAME Eigen
  VERSION 3.2.8
  URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
  # Eigen's CMakelists are not intended for library use
  DOWNLOAD_ONLY YES
)
if(Eigen_ADDED)
  add_library(Eigen INTERFACE IMPORTED)
  target_include_directories(Eigen INTERFACE ${Eigen_SOURCE_DIR})
endif()
if(TEST_INSTALLED_VERSION)
  find_package(RayTracerChallenge REQUIRED)
else()
  CPMAddPackage(NAME RayTracerChallenge SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

# ---- Create binary ----

file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_executable(RayTracerChallengeTests ${sources})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
target_link_libraries(RayTracerChallengeTests doctest::doctest RayTracerChallenge)
set_target_properties(RayTracerChallengeTests PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(RayTracerChallenge PRIVATE -Wall -Wpedantic -Wextra -Werror)
  elseif(MSVC)
    target_compile_options(RayTracerChallenge PRIVATE /W4 /WX /wd4127)
    target_compile_definitions(RayTracerChallengeTests PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add RayTracerChallengeTests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME rayTracerChallengeTests COMMAND
# RayTracerChallengeTests)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(RayTracerChallengeTests)

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(RayTracerChallenge PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(RayTracerChallenge PUBLIC -fprofile-arcs -ftest-coverage)
endif()
