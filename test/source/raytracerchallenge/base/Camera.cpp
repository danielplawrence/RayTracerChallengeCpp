#define _USE_MATH_DEFINES
#include <doctest/doctest.h>
#include <raytracerchallenge/base/Camera.h>

#include <cmath>

using namespace raytracerchallenge;

TEST_CASE("Camera") {
  using namespace raytracerchallenge;
  SUBCASE("Constructing a camera") {
    auto camera = Camera(160, 120, M_PI / 2.0);
    CHECK(camera.hSize == 160);
    CHECK(camera.vSize == 120);
    CHECK(camera.fieldOfView == M_PI / 2.0);
    CHECK(camera.transform == Matrix::identity(4));
  }
  SUBCASE("The pixel size for a horizontal canvas") {
    auto camera = Camera(200, 125, M_PI / 2.0);
    CHECK(abs(camera.pixelSize - 0.01) < 0.0001);
  }
  SUBCASE("The pixel size for a vertical canvas") {
    auto camera = Camera(125, 200, M_PI / 2.0);
    CHECK(abs(camera.pixelSize - 0.01) < 0.0001);
  }
  SUBCASE("Constructing a ray through the center of the canvas") {
    auto camera = Camera(201, 101, M_PI / 2.0);
    auto ray = camera.rayForPixel(100, 50);
    CHECK(ray.origin == Tuple::point(0.0, 0.0, 0.0));
    CHECK(ray.direction == Tuple::vector(0.0, 0.0, -1.0));
  }
  SUBCASE("Constructing a ray through a corner of the canvas") {
    auto camera = Camera(201, 101, M_PI / 2.0);
    auto ray = camera.rayForPixel(0, 0);
    CHECK(ray.origin == Tuple::point(0.0, 0.0, 0.0));
    CHECK(ray.direction == Tuple::vector(0.66519, 0.33259, -0.66851));
  }
  SUBCASE("Constructing a ray when the camera is transformed") {
    auto camera = Camera(201, 101, M_PI / 2.0);
    camera.transform = Matrix::identity(4).translated(0.0, -2.0, 5.0).rotatedY(M_PI / 4.0);
    auto ray = camera.rayForPixel(100, 50);
    CHECK(ray.origin == Tuple::point(0.0, 2.0, -5.0));
    CHECK(ray.direction == Tuple::vector(sqrt(2.0) / 2.0, 0.0, -sqrt(2.0) / 2.0));
  }
  SUBCASE("Rendering a world with a camera") {
    auto world = World::defaultWorld();
    auto camera = Camera(11, 11, M_PI / 2.0);
    auto from = Tuple::point(0.0, 0.0, -5.0);
    auto to = Tuple::point(0.0, 0.0, 0.0);
    auto up = Tuple::vector(0.0, 1.0, 0.0);
    camera.transform = Matrix::view(from, to, up);
    auto image = camera.render(world);
    CHECK(image.pixelAt(5, 5) == Color(0.38066, 0.47583, 0.2855));
  }
}