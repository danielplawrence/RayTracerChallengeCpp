#define _USE_MATH_DEFINES
#include <doctest/doctest.h>
#include <raytracerchallenge/shapes/Sphere.h>

#include <cmath>

using namespace raytracerchallenge;

TEST_CASE("Spheres") {
  SUBCASE("A Sphere has a default material") {
    auto sphere = Sphere::create();
    Material material;
    CHECK(*sphere->material == material);
  }
  SUBCASE("A Sphere may be assigned a material") {
    auto sphere = Sphere::create();
    auto material = std::shared_ptr<Material>(new Material());
    material->ambient = 1.0;
    sphere->material = material;
    CHECK(sphere->material == material);
  }
  SUBCASE("A sphere's default transformation") {
    auto sphere = Sphere::create();
    CHECK(sphere->transform == Matrix::identity(4));
  }
  SUBCASE("Changing a sphere's transformation") {
    auto sphere = Sphere::create();
    Matrix matrix = Matrix::translation(2.0, 3.0, 4.0);
    sphere->transform = matrix;
    CHECK(sphere->transform == matrix);
  }
  SUBCASE("The normal on a sphere at a point on the x axis") {
    auto sphere = Sphere::create();
    auto n = sphere->normalAt(Tuple::point(1.0, 0.0, 0.0), {});
    CHECK(n == Tuple::vector(1.0, 0.0, 0.0));
  }
  SUBCASE("The normal on a sphere at a point on the y axis") {
    auto sphere = Sphere::create();
    auto n = sphere->normalAt(Tuple::point(0.0, 1.0, 0.0), {});
    CHECK(n == Tuple::vector(0.0, 1.0, 0.0));
  }
  SUBCASE("The normal on a sphere at a point on the z axis") {
    auto sphere = Sphere::create();
    auto n = sphere->normalAt(Tuple::point(0.0, 0.0, 1.0), {});
    CHECK(n == Tuple::vector(0.0, 0.0, 1.0));
  }
  SUBCASE("The normal on a sphere at a nonaxial point") {
    auto sphere = Sphere::create();
    auto n = sphere->normalAt(Tuple::point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0), {});
    CHECK(n == Tuple::vector(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
  }
  SUBCASE("The normal is a normalized vector") {
    auto sphere = Sphere::create();
    auto n = sphere->normalAt(Tuple::point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0), {});
    CHECK(n == n.normalize());
  }
  SUBCASE("Computing the normal on a translated sphere") {
    auto sphere = Sphere::create();
    sphere->transform = Matrix::translation(0.0, 1.0, 0.0);
    auto n = sphere->normalAt(Tuple::point(0.0, 1.70711, -0.70711), {});
    CHECK(n == Tuple::vector(0.0, 0.70711, -0.70711));
  }
  SUBCASE("Computing the normal on a transformed sphere") {
    auto sphere = Sphere::create();
    sphere->transform = (Matrix::scaling(1.0, 0.5, 1.0) * Matrix::rotationZ(M_PI / 5.0));
    auto n = sphere->normalAt(Tuple::point(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0), {});
    CHECK(n == Tuple::vector(0.0, 0.97014, -0.24254));
  }
  SUBCASE("A sphere has a bounding box") {
    auto sphere = Sphere::create();
    auto b = sphere->bounds();
    CHECK(b.min == Tuple(-1.0, -1.0, -1.0, 1.0));
    CHECK(b.max == Tuple(1.0, 1.0, 1.0, 1.0));
  }
}