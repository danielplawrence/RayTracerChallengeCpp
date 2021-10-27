#include <doctest/doctest.h>
#include <raytracerchallenge/base/Color.h>
#include <raytracerchallenge/base/Light.h>
#include <raytracerchallenge/patterns/StripePattern.h>
#include <raytracerchallenge/shapes/Sphere.h>

using namespace raytracerchallenge;

TEST_CASE("Lighting") {
  SUBCASE("Lighting with the eye between the light and the surface") {
    auto s = Sphere::create();
    auto position = Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = Tuple::vector(0.0, 0.0, -1.0);
    auto light = PointLight(Tuple::point(0.0, 0.0, -10.0), Color(1.0, 1.0, 1.0));
    auto result = lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == Color(1.9, 1.9, 1.9));
  }
  SUBCASE("Lighting with the eye between the light and the surface, eye offset 45 degrees") {
    auto s = Sphere::create();
    auto position = Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = Tuple::vector(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
    auto normalVector = Tuple::vector(0.0, 0.0, -1.0);
    auto light = PointLight(Tuple::point(0.0, 0.0, -10.0), Color(1.0, 1.0, 1.0));
    auto result = lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == Color(1.0, 1.0, 1.0));
  }
  SUBCASE("Lighting with the eye opposite surface, light offset 45 degrees") {
    auto s = Sphere::create();
    auto position = Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = Tuple::vector(0.0, 0.0, -1.0);
    auto light = PointLight(Tuple::point(0.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));
    auto result = lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == Color(0.7364, 0.7364, 0.7364));
  }
  SUBCASE("Lighting with eye in the path of the reflection vector") {
    auto s = Sphere::create();
    auto position = Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = Tuple::vector(0.0, -sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
    auto normalVector = Tuple::vector(0.0, 0.0, -1.0);
    auto light = PointLight(Tuple::point(0.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));
    auto result = lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == Color(1.63638, 1.63638, 1.63638));
  }
  SUBCASE("Lighting with the light behind the surface") {
    auto s = Sphere::create();
    auto position = Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = Tuple::vector(0.0, 0.0, -1.0);
    auto light = PointLight(Tuple::point(0.0, 0.0, 10.0), Color(1.0, 1.0, 1.0));
    auto result = lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == Color(0.1, 0.1, 0.1));
  }
  SUBCASE("Lighting with the surface in shadow") {
    auto s = Sphere::create();
    auto position = Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = Tuple::vector(0.0, 0.0, -1.0);
    auto light = PointLight(Tuple::point(0.0, 0.0, -10.0), Color(1.0, 1.0, 1.0));
    auto result = lighting(s, light, position, eyeVector, normalVector, true);
    CHECK(result == Color(0.1, 0.1, 0.1));
  }
  SUBCASE("Lighting with a pattern applied") {
    auto s = Sphere::create();
    auto pattern = StripePattern(WHITE, BLACK);
    s->material.pattern = &pattern;
    s->material.ambient = 1.0;
    s->material.diffuse = 0.0;
    s->material.specular = 0.0;
    auto eyeVector = Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = Tuple::vector(0.0, 0.0, -1.0);
    auto light = PointLight(Tuple::point(0.0, 0.0, -10.0), Color(1.0, 1.0, 1.0));
    auto c1 = lighting(s, light, {0.9, 0.0, 0.0, 1.0}, eyeVector, normalVector, false);
    auto c2 = lighting(s, light, {1.1, 0.0, 0.0, 1.0}, eyeVector, normalVector, false);
    CHECK(c1 == WHITE);
    CHECK(c2 == BLACK);
  }
}
TEST_CASE("Point Lights") {
  SUBCASE("A point light has a position and intensity") {
    auto intensity = Color(1.0, 1.0, 1.0);
    auto position = Tuple::point(0.0, 0.0, 0.0);
    auto light = PointLight(position, intensity);
    CHECK(light.intensity == intensity);
    CHECK(light.position == position);
  }
}