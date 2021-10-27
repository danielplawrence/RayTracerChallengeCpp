#include <doctest/doctest.h>
#include <raytracerchallenge/patterns/GradientPattern.h>
#include <raytracerchallenge/shapes/Sphere.h>

using namespace raytracerchallenge;

TEST_CASE("Gradient") {
  SUBCASE("A gradient linearly interpolates between colors") {
    auto shape = Sphere::create();
    auto pattern = GradientPattern(WHITE, BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {0.25, 0.0, 0.0, 1.0}) == Color(0.75, 0.75, 0.75));
    CHECK(pattern.colorAt(shape, {0.5, 0.0, 0.0, 1.0}) == Color(0.5, 0.5, 0.5));
    CHECK(pattern.colorAt(shape, {0.75, 0.0, 0.0, 1.0}) == Color(0.25, 0.25, 0.25));
  }
}