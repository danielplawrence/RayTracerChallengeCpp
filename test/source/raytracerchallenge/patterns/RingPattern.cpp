#include <doctest/doctest.h>
#include <raytracerchallenge/patterns/RingPattern.h>
#include <raytracerchallenge/shapes/Sphere.h>

using namespace raytracerchallenge;

TEST_CASE("Rings") {
  SUBCASE("A ring should extend in both x and z") {
    auto shape = Sphere::create();
    auto pattern = RingPattern(WHITE, BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {1.0, 0.0, 0.0, 1.0}) == BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 1.0, 1.0}) == BLACK);
    CHECK(pattern.colorAt(shape, {0.708, 0.0, 0.708, 1.0}) == BLACK);
  }
}