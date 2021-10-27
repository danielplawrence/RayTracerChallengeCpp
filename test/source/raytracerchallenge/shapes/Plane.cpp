#include <doctest/doctest.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Plane.h>

using namespace raytracerchallenge;

TEST_CASE("Planes") {
  SUBCASE("The normal of a plane is constant everywhere") {
    auto plane = Plane::create();
    auto n1 = plane->localNormalAt({0.0, 0.0, 0.0, 0.0}, {});
    auto n2 = plane->localNormalAt({10.0, 0.0, -10.0, 0.0}, {});
    auto n3 = plane->localNormalAt({-5.0, 0.0, 150.0, 0.0}, {});
    CHECK(n1 == Tuple::vector(0.0, 1.0, 0.0));
    CHECK(n2 == Tuple::vector(0.0, 1.0, 0.0));
    CHECK(n3 == Tuple::vector(0.0, 1.0, 0.0));
  }
  SUBCASE("Intersect with a ray parallel to the plane") {
    Tuple origin = Tuple::point(0.0, 10.0, 1.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    Ray ray(origin, direction);
    auto plane = Plane::create();
    auto xs = plane->localIntersect(ray);
    CHECK(xs.size() == 0);
  }
  SUBCASE("Intersect with a coplanar ray") {
    Tuple origin = Tuple::point(0.0, 0.0, 0.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    Ray ray(origin, direction);
    auto plane = Plane::create();
    auto xs = plane->localIntersect(ray);
    CHECK(xs.size() == 0);
  }
  SUBCASE("A ray intersecting a plane from above") {
    Tuple origin = Tuple::point(0.0, 1.0, 0.0);
    Tuple direction = Tuple::vector(0.0, -1.0, 0.0);
    Ray ray(origin, direction);
    auto plane = Plane::create();
    auto xs = plane->localIntersect(ray);
    CHECK(xs.size() == 1);
    CHECK(xs[0].t == 1);
    CHECK(xs[0].object == plane);
  }
  SUBCASE("A ray intersecting a plane from below") {
    Tuple origin = Tuple::point(0.0, -1.0, 0.0);
    Tuple direction = Tuple::vector(0.0, 1.0, 0.0);
    Ray ray(origin, direction);
    auto plane = Plane::create();
    auto xs = plane->localIntersect(ray);
    CHECK(xs.size() == 1);
    CHECK(xs[0].t == 1);
    CHECK(xs[0].object == plane);
  }
  SUBCASE("A plane has a bounding box") {
    auto plane = Plane::create();
    auto b = plane->bounds();
    CHECK(b.min == Tuple(-INFINITY, 0.0, -INFINITY, 1.0));
    CHECK(b.max == Tuple(INFINITY, 0.0, INFINITY, 1.0));
  }
}