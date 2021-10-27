#include <doctest/doctest.h>
#include <raytracerchallenge/patterns/StripePattern.h>
#include <raytracerchallenge/shapes/Sphere.h>

using namespace raytracerchallenge;

TEST_CASE("Stripes") {
  SUBCASE("A stripe pattern is constant in y") {
    auto shape = Sphere::create();
    auto pattern = StripePattern(WHITE, BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 1.0, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 2.0, 0.0, 1.0}) == WHITE);
  }
  SUBCASE("A stripe pattern is constant in z") {
    auto shape = Sphere::create();
    auto pattern = StripePattern(WHITE, BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 1.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 2.0, 1.0}) == WHITE);
  }
  SUBCASE("A stripe pattern alternates in x") {
    auto pattern = StripePattern(WHITE, BLACK);
    auto shape = Sphere::create();
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {0.9, 0.0, 0.0, 1.0}) == WHITE);
    CHECK(pattern.colorAt(shape, {1.0, 0.0, 0.0, 1.0}) == BLACK);
    CHECK(pattern.colorAt(shape, {-0.1, 0.0, 0.0, 1.0}) == BLACK);
    CHECK(pattern.colorAt(shape, {-1.0, 0.0, 0.0, 1.0}) == BLACK);
    CHECK(pattern.colorAt(shape, {-1.1, 0.0, 0.0, 1.0}) == WHITE);
  }
  SUBCASE("Stripes with an object transformation") {
    auto object = Sphere::create();
    object->transform = Matrix::scaling(2.0, 2.0, 2.0);
    auto pattern = StripePattern(WHITE, BLACK);
    CHECK(pattern.colorAt(object, {1.5, 0.0, 0.0, 1.0}) == WHITE);
  }
  SUBCASE("Stripes with an pattern transformation") {
    auto object = Sphere::create();
    auto pattern = StripePattern(WHITE, BLACK);
    pattern.transform = Matrix::scaling(2.0, 2.0, 2.0);
    CHECK(pattern.colorAt(object, {1.5, 0.0, 0.0, 1.0}) == WHITE);
  }
  SUBCASE("Stripes with both an object and pattern transformation") {
    auto object = Sphere::create();
    auto pattern = StripePattern(WHITE, BLACK);
    object->transform = Matrix::scaling(2.0, 2.0, 2.0);
    pattern.transform = Matrix::translation(0.5, 0.0, 0.0);
    CHECK(pattern.colorAt(object, {2.5, 0.0, 0.0, 1.0}) == WHITE);
  }
}