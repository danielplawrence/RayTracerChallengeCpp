#include <doctest/doctest.h>
#include <raytracerchallenge/raytracerchallenge.h>

TEST_CASE("Tuples") {
  using namespace raytracerchallenge;
  SUBCASE("A tuple with w=0.1 is a point") {
    RayTracerChallenge::Tuple tuple = RayTracerChallenge::Tuple(4.3, -4.2, 3.1, 1.0);
    CHECK(tuple.x == 4.3f);
    CHECK(tuple.y == -4.2f);
    CHECK(tuple.z == 3.1f);
    CHECK(tuple.w == 1.0f);
    CHECK(tuple.isVector() == false);
  }
  SUBCASE("A tuple with w=0.0 is a vector") {
    RayTracerChallenge::Tuple tuple = RayTracerChallenge::Tuple(4.3, -4.2, 3.1, 0.0);
    CHECK(tuple.x == 4.3f);
    CHECK(tuple.y == -4.2f);
    CHECK(tuple.z == 3.1f);
    CHECK(tuple.w == 0.0f);
    CHECK(tuple.isVector() == true);
  }
  SUBCASE("point() creates tuples with w=1") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(4, -4, 3);
    CHECK(point == RayTracerChallenge::Tuple(4, -4, 3, 1.0));
  }
  SUBCASE("vector() creates tuples with w=0") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::vector(4, -4, 3);
    CHECK(point == RayTracerChallenge::Tuple(4, -4, 3, 0.0f));
  }
}