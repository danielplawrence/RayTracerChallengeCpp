#define _USE_MATH_DEFINES
#include <doctest/doctest.h>
#include <raytracerchallenge/base/Tuple.h>

#include <cmath>

using namespace raytracerchallenge;

TEST_CASE("Tuples") {
  SUBCASE("The default tuple initializes with all values set to zero") {
    Tuple tuple;
    CHECK(tuple.x == 0.0);
    CHECK(tuple.y == 0.0);
    CHECK(tuple.z == 0.0);
    CHECK(tuple.w == 0.0);
  }
  SUBCASE("A tuple with w=0.1 is a point") {
    Tuple tuple = Tuple(4.3, -4.2, 3.1, 1.0);
    CHECK(tuple.x == 4.3);
    CHECK(tuple.y == -4.2);
    CHECK(tuple.z == 3.1);
    CHECK(tuple.w == 1.0);
    CHECK(tuple.isVector() == false);
  }
  SUBCASE("A tuple with w=0.0 is a vector") {
    Tuple tuple = Tuple(4.3, -4.2, 3.1, 0.0);
    CHECK(tuple.x == 4.3);
    CHECK(tuple.y == -4.2);
    CHECK(tuple.z == 3.1);
    CHECK(tuple.w == 0.0);
    CHECK(tuple.isVector() == true);
  }
  SUBCASE("point() creates tuples with w=1") {
    Tuple point = Tuple::point(4, -4, 3);
    CHECK(point == Tuple(4, -4, 3, 1.0));
  }
  SUBCASE("vector() creates tuples with w=0") {
    Tuple point = Tuple::vector(4, -4, 3);
    CHECK(point == Tuple(4, -4, 3, 0.0));
  }
  SUBCASE("Adding two tuples") {
    Tuple t1 = Tuple(3, -2, 5, 1);
    Tuple t2 = Tuple(-2, 3, 1, 0);
    Tuple t3 = t1 + t2;
    CHECK(t3 == Tuple(1, 1, 6, 1));
  }
  SUBCASE("Subtracting two points") {
    Tuple p1 = Tuple::point(3, 2, 1);
    Tuple p2 = Tuple::point(5, 6, 7);
    Tuple p3 = p1 - p2;
    CHECK(p3 == Tuple::vector(-2, -4, -6));
  }
  SUBCASE("Subtracting a vector from a point") {
    Tuple p = Tuple::point(3, 2, 1);
    Tuple v = Tuple::vector(5, 6, 7);
    CHECK(p - v == Tuple::point(-2, -4, -6));
  }
  SUBCASE("Subtracting two vectors") {
    Tuple v1 = Tuple::vector(3, 2, 1);
    Tuple v2 = Tuple::vector(5, 6, 7);
    CHECK(v1 - v2 == Tuple::vector(-2, -4, -6));
  }
  SUBCASE("Subtracting a vector from the zero vector") {
    Tuple zero = Tuple::vector(0, 0, 0);
    Tuple v = Tuple::vector(1, -2, 3);
    CHECK(zero - v == Tuple::vector(-1, 2, -3));
  }
  SUBCASE("Negating a tuple") {
    Tuple t = Tuple(1, -2, 3, -4);
    CHECK(-t == Tuple(-1, 2, -3, 4));
  }
  SUBCASE("Multiplying a tuple by a scalar") {
    Tuple t = Tuple(1, -2, 3, -4);
    CHECK(t * 3.5 == Tuple(3.5, -7, 10.5, -14));
  }
  SUBCASE("Multiplying a tuple by a fraction") {
    Tuple t = Tuple(1, -2, 3, -4);
    CHECK(t * 0.5 == Tuple(0.5, -1, 1.5, -2));
  }
  SUBCASE("Dividing a tuple by a scalar") {
    Tuple t = Tuple(1, -2, 3, -4);
    CHECK(t / 2 == Tuple(0.5, -1, 1.5, -2));
  }
  SUBCASE("Computing the magnitude of a vector(1, 0, 0)") {
    Tuple v = Tuple::vector(1, 0, 0);
    CHECK(v.magnitude() == 1);
  }
  SUBCASE("Computing the magnitude of a vector(0, 1, 0)") {
    Tuple v = Tuple::vector(0, 1, 0);
    CHECK(v.magnitude() == 1);
  }
  SUBCASE("Computing the magnitude of a vector(0, 0, 1)") {
    Tuple v = Tuple::vector(0, 0, 1);
    CHECK(v.magnitude() == 1);
  }
  SUBCASE("Computing the magnitude of a vector(1, 2, 3)") {
    Tuple v = Tuple::vector(1, 2, 3);
    CHECK(abs(v.magnitude() - sqrt(14)) < 0.000001);
  }
  SUBCASE("Computing the magnitude of a vector(-1, -2, -3)") {
    Tuple v = Tuple::vector(1, 2, 3);
    CHECK(abs(v.magnitude() - sqrt(14)) < 0.000001);
  }
  SUBCASE("Normalizing vector(4, 0, 0) gives (1, 0, 0)") {
    Tuple v = Tuple::vector(4, 0, 0);
    CHECK(v.normalize() == Tuple::vector(1, 0, 0));
  }
  SUBCASE("Normalizing vector(1, 2, 3)") {
    Tuple v = Tuple::vector(1, 2, 3);
    CHECK(v.normalize() == Tuple::vector(1 / sqrt(14), 2 / sqrt(14), 3 / sqrt(14)));
  }
  SUBCASE("The magnitude of a normalized vector") {
    Tuple v = Tuple::vector(1, 2, 3);
    Tuple v1 = v.normalize();
    CHECK(abs(v1.magnitude() - 1) < 0.000001);
  }
  SUBCASE("The dot product of two tuples") {
    Tuple a = Tuple::vector(1, 2, 3);
    Tuple b = Tuple::vector(2, 3, 4);
    CHECK(a.dot(b) == 20);
  }
  SUBCASE("The cross product of two vectors") {
    Tuple a = Tuple::vector(1, 2, 3);
    Tuple b = Tuple::vector(2, 3, 4);
    CHECK(a.cross(b) == Tuple::vector(-1, 2, -1));
    CHECK(b.cross(a) == Tuple::vector(1, -2, 1));
  }
  SUBCASE("Reflecting a vector approaching at 45 degrees") {
    auto v = Tuple::vector(1.0, -1.0, 0.0);
    auto n = Tuple::vector(0.0, 1.0, 0.0);
    auto r = v.reflect(n);
    CHECK(r == Tuple::vector(1.0, 1.0, 0.0));
  }
  SUBCASE("Reflecting a vector off a slanted surface") {
    auto v = Tuple::vector(0.0, -1.0, 0.0);
    auto n = Tuple::vector(sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0);
    auto r = v.reflect(n);
    CHECK(r == Tuple::vector(1.0, 0.0, 0.0));
  }
}