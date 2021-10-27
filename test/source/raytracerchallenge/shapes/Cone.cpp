#include <doctest/doctest.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Cone.h>

using namespace raytracerchallenge;

static Intersections getIntersectionsForCone(const std::shared_ptr<Shape>& cone, Tuple origin,
                                             Tuple direction) {
  auto r = Ray(origin, direction);
  return cone->localIntersect(r);
}

TEST_CASE("Cones") {
  SUBCASE("Intersecting a cone with a ray") {
    auto cone = Cone::create();
    auto res1 = getIntersectionsForCone(cone, Tuple(0.0, 0.0, -5.0, 1.0),
                                        Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res1[0].t == 5.0);
    CHECK(res1[1].t == 5.0);
    auto res2 = getIntersectionsForCone(cone, Tuple(0.0, 0.0, -5.0, 1.0),
                                        Tuple(1.0, 1.0, 1.0, 0.0).normalize());
    CHECK(abs(res2[0].t - 8.66025) < 0.0001);
    CHECK(abs(res2[1].t - 8.66025) < 0.0001);
    auto res3 = getIntersectionsForCone(cone, Tuple(1.0, 1.0, -5.0, 1.0),
                                        Tuple(-0.5, -1.0, 1.0, 0.0).normalize());
    CHECK(abs(res3[0].t - 4.55006) < 0.0001);
    CHECK(abs(res3[1].t - 49.44994) < 0.0001);
    auto res4 = getIntersectionsForCone(cone, Tuple(0.0, 0.0, -1.0, 1.0),
                                        Tuple(0.0, 1.0, 1.0, 0.0).normalize());
    CHECK(abs(res4[0].t - 0.35355) < 0.0001);
  }
  SUBCASE("Intersecting a cone's end caps") {
    auto cone = Cone::create(-0.5, 0.5, true);
    auto res1 = getIntersectionsForCone(cone, Tuple(0.0, 0.0, -5.0, 1.0),
                                        Tuple(0.0, 1.0, 0.0, 0.0).normalize());
    CHECK(res1.size() == 0);
    auto res2 = getIntersectionsForCone(cone, Tuple(0.0, 0.0, -0.25, 1.0),
                                        Tuple(0.0, 1.0, 1.0, 0.0).normalize());
    CHECK(res2.size() == 2);
    auto res3 = getIntersectionsForCone(cone, Tuple(0.0, 0.0, -0.25, 1.0),
                                        Tuple(0.0, 1.0, 0.0, 0.0).normalize());
    CHECK(res3.size() == 4);
  }
  SUBCASE("Computing the normal vector on a cone") {
    auto cone = Cone::create();
    CHECK(cone->localNormalAt({0.0, 0.0, 0.0, 1.0}, {}) == Tuple(0.0, 0.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({1.0, 1.0, 1.0, 1.0}, {}) == Tuple(1.0, -sqrt(2.0), 1.0, 0.0));
    CHECK(cone->localNormalAt({-1.0, -1.0, 0.0, 1.0}, {}) == Tuple(-1.0, 1.0, 0.0, 0.0));
  }
  SUBCASE("An unbounded cone has a bounding box") {
    auto cone = Cone::create();
    auto b = cone->bounds();
    CHECK(b.min == Tuple(-INFINITY, -INFINITY, -INFINITY, 1.0));
    CHECK(b.max == Tuple(INFINITY, INFINITY, INFINITY, 1.0));
  }
  SUBCASE("A bounded cone has a bounding box") {
    auto cone = Cone::create(-5.0, 3.0, true);
    auto b = cone->bounds();
    CHECK(b.min == Tuple(-5.0, -5.0, -5.0, 1.0));
    CHECK(b.max == Tuple(5.0, 3.0, 5.0, 1.0));
  }
}