#include <doctest/doctest.h>
#include <raytracerchallenge/base/Matrix.h>
#include <raytracerchallenge/base/Ray.h>
#include <raytracerchallenge/base/Tuple.h>

using namespace raytracerchallenge;

TEST_CASE("Rays") {
  SUBCASE("Creating and querying a ray") {
    Tuple origin = Tuple::point(1.0, 2.0, 3.0);
    Tuple direction = Tuple::vector(4.0, 5.0, 6.0);
    Ray ray(origin, direction);
    CHECK(ray.origin == origin);
    CHECK(ray.direction == direction);
  }
  SUBCASE("Computing a point from a distance") {
    Tuple origin = Tuple::point(2.0, 3.0, 4.0);
    Tuple direction = Tuple::vector(1.0, 0.0, 0.0);
    Ray ray(origin, direction);
    CHECK(ray.position(0.0) == Tuple::point(2.0, 3.0, 4.0));
    CHECK(ray.position(1.0) == Tuple::point(3.0, 3.0, 4.0));
    CHECK(ray.position(-1.0) == Tuple::point(1.0, 3.0, 4.0));
    CHECK(ray.position(2.5) == Tuple::point(4.5, 3.0, 4.0));
  }
  SUBCASE("Translating a ray") {
    Tuple origin = Tuple::point(1.0, 2.0, 3.0);
    Tuple direction = Tuple::vector(0.0, 1.0, 0.0);
    Ray ray(origin, direction);
    Matrix matrix = Matrix::translation(3.0, 4.0, 5.0);
    Ray ray2 = ray.transform(matrix);
    CHECK(ray2.origin == Tuple::point(4.0, 6.0, 8.0));
    CHECK(ray2.direction == Tuple::vector(0.0, 1.0, 0.0));
  }
  SUBCASE("Scaling a ray") {
    Tuple origin = Tuple::point(1.0, 2.0, 3.0);
    Tuple direction = Tuple::vector(0.0, 1.0, 0.0);
    Ray ray(origin, direction);
    Matrix matrix = Matrix::scaling(2.0, 3.0, 4.0);
    Ray ray2 = ray.transform(matrix);
    CHECK(ray2.origin == Tuple::point(2.0, 6.0, 12.0));
    CHECK(ray2.direction == Tuple::vector(0.0, 3.0, 0.0));
  }
}