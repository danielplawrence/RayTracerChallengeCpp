#include <doctest/doctest.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Triangle.h>

using namespace raytracerchallenge;

TEST_CASE("Triangles") {
  SUBCASE("Normal vector for a triangle") {
    auto t = Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0});
    auto n1 = t->localNormalAt({0.0, 0.5, 0.0, 1.0}, {});
    auto n2 = t->localNormalAt({-0.5, 0.75, 0.0, 1.0}, {});
    auto n3 = t->localNormalAt({0.5, 0.25, 0.0, 1.0}, {});
    auto normal = std::dynamic_pointer_cast<Triangle>(t)->normal;
    CHECK(n1 == normal);
    CHECK(n2 == normal);
    CHECK(n3 == normal);
  }
  SUBCASE("Intersecting a ray parallel to the triangle") {
    auto t = Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0});
    auto r = Ray({0.0, -1.0, -2.0, 1.0}, {0.0, 1.0, 0.0, 0.0});
    CHECK(t->localIntersect(r).size() == 0);
  }
  SUBCASE("A ray misses the p1-p3 edge") {
    auto t = Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0});
    auto r = Ray({1.0, 1.0, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    CHECK(t->localIntersect(r).size() == 0);
  }
  SUBCASE("A ray misses the p1-p2 edge") {
    auto t = Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0});
    auto r = Ray({-1.0, 1.0, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    CHECK(t->localIntersect(r).size() == 0);
  }
  SUBCASE("A ray misses the p2-p3 edge") {
    auto t = Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0});
    auto r = Ray({0.0, -1.0, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    CHECK(t->localIntersect(r).size() == 0);
  }
  SUBCASE("A ray strikes a triangle") {
    auto t = Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0});
    auto r = Ray({0.0, 0.5, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = t->localIntersect(r);
    CHECK(xs.size() == 1);
    CHECK(xs[0].t == 2.0);
  }
  SUBCASE("A triangle has a bounding box") {
    auto p1 = Tuple::point(-3.0, 7.0, 2.0);
    auto p2 = Tuple::point(6.0, 2.0, -4.0);
    auto p3 = Tuple::point(2.0, -1.0, -1.0);
    auto t = Triangle::create(p1, p2, p3);
    auto box = t->bounds();
    CHECK(box.max == Tuple::point(6.0, 7.0, 2.0));
    CHECK(box.min == Tuple::point(-3.0, -1.0, -4.0));
  }
}