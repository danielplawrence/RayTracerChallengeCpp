cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(RayTracerChallengeStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 2.2.1
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

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

CPMAddPackage(NAME RayTracerChallenge SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(RayTracerChallengeStandalone ${sources})

set_target_properties(
  RayTracerChallengeStandalone PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "RayTracerChallenge"
)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread -ffast-math -O3 -DNDEBUG")

target_link_libraries(
  RayTracerChallengeStandalone RayTracerChallenge::RayTracerChallenge Eigen cxxopts
)
