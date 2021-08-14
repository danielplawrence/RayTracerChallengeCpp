#include <doctest/doctest.h>
#include <raytracerchallenge/raytracerchallenge.h>
#include <raytracerchallenge/version.h>

#include <string>

TEST_CASE("RayTracerChallenge") {
  using namespace raytracerchallenge;

  RayTracerChallenge raytracerchallenge("Tests");

  CHECK(raytracerchallenge.greet(LanguageCode::EN) == "Hello, Tests!");
  CHECK(raytracerchallenge.greet(LanguageCode::DE) == "Hallo Tests!");
  CHECK(raytracerchallenge.greet(LanguageCode::ES) == "¡Hola Tests!");
  CHECK(raytracerchallenge.greet(LanguageCode::FR) == "Bonjour Tests!");
}

TEST_CASE("raytracerchallenge version") {
  static_assert(std::string_view(RAYTRACERCHALLENGE_VERSION) == std::string_view("1.0"));
  CHECK(std::string(RAYTRACERCHALLENGE_VERSION) == std::string("1.0"));
}
