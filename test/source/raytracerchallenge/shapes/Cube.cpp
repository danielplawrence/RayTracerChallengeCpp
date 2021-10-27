#include <doctest/doctest.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Cube.h>

using namespace raytracerchallenge;

static Intersections getIntersectionsForCube(const std::shared_ptr<Shape>& cube, Tuple origin,
                                             Tuple direction) {
  auto r = Ray(origin, direction);
  return cube->localIntersect(r);
}

TEST_CASE("Cubes") {
  SUBCASE("A ray intersects a cube") {
    auto cube = Cube::create();
    auto res1 = getIntersectionsForCube(cube, {5.0, 0.5, 0.0, 1.0}, {-1.0, 0.0, 0.0, 0.0});
    CHECK(res1[0].t == 4.0);
    CHECK(res1[1].t == 6.0);
    auto res2 = getIntersectionsForCube(cube, {-5.0, 0.5, 0.0, 1.0}, {1.0, 0.0, 0.0, 0.0});
    CHECK(res2[0].t == 4.0);
    CHECK(res2[1].t == 6.0);
    auto res3 = getIntersectionsForCube(cube, {0.5, 5.0, 0.0, 1.0}, {0.0, -1.0, 0.0, 0.0});
    CHECK(res3[0].t == 4.0);
    CHECK(res3[1].t == 6.0);
    auto res4 = getIntersectionsForCube(cube, {0.5, -5.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 0.0});
    CHECK(res4[0].t == 4.0);
    CHECK(res4[1].t == 6.0);
    auto res5 = getIntersectionsForCube(cube, {0.5, 0.0, 5.0, 1.0}, {0.0, 0.0, -1.0, 0.0});
    CHECK(res5[0].t == 4.0);
    CHECK(res5[1].t == 6.0);
    auto res6 = getIntersectionsForCube(cube, {0.5, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    CHECK(res6[0].t == 4.0);
    CHECK(res6[1].t == 6.0);
    auto res7 = getIntersectionsForCube(cube, {0.0, 0.5, 0.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    CHECK(res7[0].t == -1.0);
    CHECK(res7[1].t == 1.0);
  }
  SUBCASE("A ray misses a cube") {
    auto cube = Cube::create();
    auto res1 = getIntersectionsForCube(cube, {-2.0, 0.0, 0.0, 1.0}, {0.2673, 0.5345, 0.8018, 0.0});
    CHECK(res1.size() == 0);
    auto res2 = getIntersectionsForCube(cube, {0.0, -2.0, 0.0, 1.0}, {0.8018, 0.2673, 0.5345, 0.0});
    CHECK(res2.size() == 0);
    auto res3 = getIntersectionsForCube(cube, {0.0, 0.0, -2.0, 1.0}, {0.5345, 0.8018, 0.2673, 0.0});
    CHECK(res3.size() == 0);
    auto res4 = getIntersectionsForCube(cube, {2.0, 0.0, 2.0, 1.0}, {0.0, 0.0, -1.0, 0.0});
    CHECK(res4.size() == 0);
    auto res5 = getIntersectionsForCube(cube, {0.0, 2.0, 2.0, 1.0}, {0.0, -1.0, 0.0, 0.0});
    CHECK(res5.size() == 0);
    auto res6 = getIntersectionsForCube(cube, {2.0, 2.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 0.0});
    CHECK(res6.size() == 0);
  }
  SUBCASE("The normal on the surface of a cube") {
    auto cube = Cube::create();
    CHECK(cube->localNormalAt({1.0, 0.5, -0.8, 0.0}, {}) == Tuple({1.0, 0.0, 0.0, 0.0}));
    CHECK(cube->localNormalAt({-1.0, -0.2, 0.9, 0.0}, {}) == Tuple({-1.0, 0.0, 0.0, 0.0}));
    CHECK(cube->localNormalAt({-0.4, 1.0, -0.1, 0.0}, {}) == Tuple({0.0, 1.0, 0.0, 0.0}));
    CHECK(cube->localNormalAt({0.3, -1.0, -0.7, 0.0}, {}) == Tuple({0.0, -1.0, 0.0, 0.0}));
    CHECK(cube->localNormalAt({-0.6, 0.3, 1.0, 0.0}, {}) == Tuple({0.0, 0.0, 1.0, 0.0}));
    CHECK(cube->localNormalAt({0.4, 0.4, -1.0, 0.0}, {}) == Tuple({0.0, 0.0, -1.0, 0.0}));
    CHECK(cube->localNormalAt({1.0, 1.0, 1.0, 0.0}, {}) == Tuple({1.0, 0.0, 0.0, 0.0}));
    CHECK(cube->localNormalAt({-1.0, -1.0, -1.0, 0.0}, {}) == Tuple({-1.0, 0.0, 0.0, 0.0}));
  }
  SUBCASE("A cube has a bounding box") {
    auto cube = Cube::create();
    auto b = cube->bounds();
    CHECK(b.min == Tuple(-1.0, -1.0, -1.0, 1.0));
    CHECK(b.max == Tuple(1.0, 1.0, 1.0, 1.0));
  }
}