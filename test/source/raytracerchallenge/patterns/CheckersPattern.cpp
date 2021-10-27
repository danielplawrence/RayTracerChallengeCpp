#include <doctest/doctest.h>
#include <raytracerchallenge/patterns/CheckersPattern.h>
#include <raytracerchallenge/shapes/Sphere.h>

using namespace raytracerchallenge;

TEST_CASE("Checkers") {
  SUBCASE("Checkers should repeat in x") {
    auto shape = Sphere::create();
    auto pattern = CheckersPattern(WHITE, BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {0.99, 0.0, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {1.01, 0.0, 0.0, 1.0}) == BLACK);
  }
  SUBCASE("Checkers should repeat in y") {
    auto shape = Sphere::create();
    auto pattern = CheckersPattern(WHITE, BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.99, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 1.01, 0.0, 1.0}) == BLACK);
  }
  SUBCASE("Checkers should repeat in z") {
    auto shape = Sphere::create();
    auto pattern = CheckersPattern(WHITE, BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.99, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 1.01, 1.0}) == BLACK);
  }
}