#include <doctest/doctest.h>
#include <raytracerchallenge/base/Computations.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/SmoothTriangle.h>

using namespace raytracerchallenge;

TEST_CASE("Smooth triangles") {
  SUBCASE("An intersection with a smooth triangle stores u and v") {
    auto t
        = SmoothTriangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0},
                                 {0.0, 1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0, 0.0}, {1.0, 0.0, 0.0, 0.0});
    auto r = Ray({-0.2, 0.3, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = t->localIntersect(r);
    CHECK(abs(xs[0].u - 0.45) < 0.001);
    CHECK(abs(xs[0].v - 0.25) < 0.001);
  }
  SUBCASE("A smooth triangle uses u/v to interpolate the normal") {
    auto t
        = SmoothTriangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0},
                                 {0.0, 1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0, 0.0}, {1.0, 0.0, 0.0, 0.0});
    auto i = Intersection(1.0, t);
    i.u = 0.45;
    i.v = 0.25;
    auto n = t->normalAt({0.0, 0.0, 0.0, 1.0}, i);
    CHECK(n == Tuple::vector(-0.5547, 0.83205, 0.0));
  }
  SUBCASE("Preparing the normal on a smooth triangle") {
    auto t
        = SmoothTriangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0},
                                 {0.0, 1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0, 0.0}, {1.0, 0.0, 0.0, 0.0});
    auto i = Intersection(1.0, t);
    i.u = 0.45;
    i.v = 0.25;
    auto r = Ray({-0.2, 0.3, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto comps = i.prepareComputations(r);
    CHECK(comps.normalVector == Tuple::vector(-0.5547, 0.83205, 0.0));
  }
}