#include <doctest/doctest.h>
#include <raytracerchallenge/base/Computations.h>
#include <raytracerchallenge/shapes/GlassSphere.h>

using namespace raytracerchallenge;

TEST_CASE("Computations") {
  SUBCASE("The Schlick approximation under total internal reflection") {
    auto shape = GlassSphere::create();
    auto r = Ray({0.0, 0.0, sqrt(2.0) / 2.0, 1.0}, {0.0, 1.0, 0.0, 0.0});
    auto xs = Intersections({{-sqrt(2.0) / 2.0, shape}, {sqrt(2.0) / 2.0, shape}});
    auto comps = xs[1].prepareComputations(r, xs);
    auto reflectance = Computations::schlick(comps);
    CHECK(reflectance == 1.0);
  }
  SUBCASE("The Schlick approximation with a perpendicular viewing angle") {
    auto shape = GlassSphere::create();
    auto r = Ray({0.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 0.0});
    auto xs = Intersections({{-1.0, shape}, {1.0, shape}});
    auto comps = xs[1].prepareComputations(r, xs);
    auto reflectance = Computations::schlick(comps);
    CHECK(abs(reflectance - 0.04) < 0.0001);
  }
  SUBCASE("The Schlick approximation with a small angle and n2 > n1") {
    auto shape = GlassSphere::create();
    auto r = Ray({0.0, 0.99, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = Intersections({{1.8589, shape}});
    auto comps = xs[0].prepareComputations(r, xs);
    auto reflectance = Computations::schlick(comps);
    CHECK(abs(reflectance - 0.48873) < 0.0001);
  }
}