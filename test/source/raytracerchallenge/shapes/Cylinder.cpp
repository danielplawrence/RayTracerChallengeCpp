#include <doctest/doctest.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Cylinder.h>

using namespace raytracerchallenge;

static Intersections getIntersectionsForCylinder(const std::shared_ptr<Shape>& cylinder,
                                                 Tuple origin, Tuple direction) {
  auto r = Ray(origin, direction);
  return cylinder->localIntersect(r);
}

TEST_CASE("Cylinders") {
  SUBCASE("A ray misses a cylinder") {
    auto cone = Cylinder::create();
    auto res1 = getIntersectionsForCylinder(cone, Tuple(1.0, 0.0, 0.0, 1.0),
                                            Tuple(0.0, 1.0, 0.0, 0.0).normalize());
    CHECK(res1.size() == 0);
    auto res2 = getIntersectionsForCylinder(cone, Tuple(0.0, 0.0, 0.0, 1.0),
                                            Tuple(0.0, 1.0, 0.0, 0.0).normalize());
    CHECK(res2.size() == 0);
    auto res3 = getIntersectionsForCylinder(cone, Tuple(0.0, 0.0, -5.0, 1.0),
                                            Tuple(1.0, 1.0, 1.0, 0.0).normalize());
    CHECK(res3.size() == 0);
  }
  SUBCASE("A ray strikes a cylinder") {
    auto cone = Cylinder::create();
    auto res1 = getIntersectionsForCylinder(cone, Tuple(1.0, 0.0, -5.0, 1.0),
                                            Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res1[0].t == 5.0);
    CHECK(res1[1].t == 5.0);
    auto res2 = getIntersectionsForCylinder(cone, Tuple(0.0, 0.0, -5.0, 1.0),
                                            Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res2[0].t == 4.0);
    CHECK(res2[1].t == 6.0);
    auto res3 = getIntersectionsForCylinder(cone, Tuple(0.5, 0.0, -5.0, 1.0),
                                            Tuple(0.1, 1.0, 1.0, 0.0).normalize());
    CHECK(abs(res3[0].t - 6.80798) < 0.0001);
    CHECK(abs(res3[1].t - 7.08872) < 0.0001);
  }
  SUBCASE("Normal vector on a cylinder") {
    auto cone = Cylinder::create();
    CHECK(cone->localNormalAt({1.0, 0.0, 0.0, 1.0}, {}) == Tuple(1.0, 0.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.0, 5.0, -1.0, 1.0}, {}) == Tuple(0.0, 0.0, -1.0, 0.0));
    CHECK(cone->localNormalAt({0.0, -2.0, 1.0, 1.0}, {}) == Tuple(0.0, 0.0, 1.0, 0.0));
    CHECK(cone->localNormalAt({-1.0, 1.0, 0.0, 1.0}, {}) == Tuple(-1.0, 0.0, 0.0, 0.0));
  }
  SUBCASE("Truncated cylinders") {
    auto cone = Cylinder::create(1.0, 2.0);
    auto res1 = getIntersectionsForCylinder(cone, Tuple(0.0, 1.5, 0.0, 1.0),
                                            Tuple(0.1, 1.0, 1.0, 0.0).normalize());
    CHECK(res1.size() == 0);
    auto res2 = getIntersectionsForCylinder(cone, Tuple(0.0, 3.0, -5.0, 1.0),
                                            Tuple(0.0, 1.0, 1.0, 0.0).normalize());
    CHECK(res2.size() == 0);
    auto res3 = getIntersectionsForCylinder(cone, Tuple(0.0, 0.0, -5.0, 1.0),
                                            Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res3.size() == 0);
    auto res4 = getIntersectionsForCylinder(cone, Tuple(0.0, 2.0, -5.0, 1.0),
                                            Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res4.size() == 0);
    auto res5 = getIntersectionsForCylinder(cone, Tuple(0.0, 1.0, -5.0, 1.0),
                                            Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res5.size() == 0);
    auto res6 = getIntersectionsForCylinder(cone, Tuple(0.0, 1.5, -2.0, 1.0),
                                            Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res6.size() == 2);
  }
  SUBCASE("Intersecting a cylinder's end caps") {
    auto cone = Cylinder::create(1.0, 2.0, true);
    auto res1 = getIntersectionsForCylinder(cone, Tuple(0.0, 3.0, 0.0, 1.0),
                                            Tuple(0.0, -1.0, 0.0, 0.0).normalize());
    CHECK(res1.size() == 2);
    auto res2 = getIntersectionsForCylinder(cone, Tuple(0.0, 3.0, -2.0, 1.0),
                                            Tuple(0.0, -1.0, 2.0, 0.0).normalize());
    CHECK(res2.size() == 2);
    auto res3 = getIntersectionsForCylinder(cone, Tuple(0.0, 4.0, -2.0, 1.0),
                                            Tuple(0.0, -1.0, 1.0, 0.0).normalize());
    CHECK(res3.size() == 2);
    auto res4 = getIntersectionsForCylinder(cone, Tuple(0.0, 0.0, -2.0, 1.0),
                                            Tuple(0.0, 1.0, 2.0, 0.0).normalize());
    CHECK(res4.size() == 2);
    auto res5 = getIntersectionsForCylinder(cone, Tuple(0.0, -1.0, -2.0, 1.0),
                                            Tuple(0.0, 1.0, 1.0, 0.0).normalize());
    CHECK(res5.size() == 2);
  }
  SUBCASE("The normal vector on a cylinder's end caps") {
    auto cone = Cylinder::create(1.0, 2.0, true);
    CHECK(cone->localNormalAt({0.0, 1.0, 0.0, 1.0}, {}) == Tuple(0.0, -1.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.5, 1.0, 0.0, 1.0}, {}) == Tuple(0.0, -1.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.0, 1.0, 0.5, 1.0}, {}) == Tuple(0.0, -1.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.0, 2.0, 0.0, 1.0}, {}) == Tuple(0.0, 1.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.5, 2.0, 0.0, 1.0}, {}) == Tuple(0.0, 1.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.0, 2.0, 0.5, 1.0}, {}) == Tuple(0.0, 1.0, 0.0, 0.0));
  }
  SUBCASE("An unbounded cylinder has a bounding box") {
    auto cyl = Cylinder::create();
    auto b = cyl->bounds();
    CHECK(b.min == Tuple(-1.0, -INFINITY, -1.0, 1.0));
    CHECK(b.max == Tuple(1.0, INFINITY, 1.0, 1.0));
  }
  SUBCASE("A bounded cylinder has a bounding box") {
    auto cyl = Cylinder::create(-5.0, 3.0, true);
    auto b = cyl->bounds();
    CHECK(b.min == Tuple(-1.0, -5.0, -1.0, 1.0));
    CHECK(b.max == Tuple(1.0, 3.0, 1.0, 1.0));
  }
}