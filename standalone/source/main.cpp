#include <raytracerchallenge/raytracerchallenge.h>
#include <raytracerchallenge/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

auto main(int argc, char** argv) -> int {
  cxxopts::Options options(*argv, "Welcome to RayTracerChallenge");
  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
  ;
  // clang-format on

  auto result = options.parse(argc, argv);

  if (result["help"].as<bool>()) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (result["version"].as<bool>()) {
    std::cout << "RayTracerChallenge, version " << RAYTRACERCHALLENGE_VERSION << std::endl;
    return 0;
  }

  return 0;
}
