#include <doctest/doctest.h>
#include <raytracerchallenge/base/World.h>
#include <raytracerchallenge/patterns/Pattern.h>
#include <raytracerchallenge/shapes/Plane.h>
#include <raytracerchallenge/shapes/Sphere.h>

using namespace raytracerchallenge;

TEST_CASE("World") {
  SUBCASE("Creating a world") {
    World world;
    CHECK(world.isEmpty());
    CHECK(world.light.has_value() == false);
  }
  SUBCASE("The default world") {
    auto world = World::defaultWorld();
    auto sphere1 = Sphere::create();
    sphere1->material.color = Color(0.8, 1.0, 0.6);
    sphere1->material.diffuse = 0.7;
    sphere1->material.specular = 0.2;
    auto sphere2 = Sphere::create();
    sphere2->transform = Matrix::scaling(0.5, 0.5, 0.5);
    CHECK(world.light->position == Tuple::point(-10.0, 10.0, -10.0));
    CHECK(world.light->intensity == Color(1.0, 1.0, 1.0));
    CHECK(*sphere1 == *world.objects[0]);
    CHECK(*sphere2 == *world.objects[1]);
  }
  SUBCASE("Intersecting a ray with the world") {
    auto world = World::defaultWorld();
    auto ray = Ray(Tuple::point(0.0, 0.0, -5.0), Tuple::vector(0.0, 0.0, 1.0));
    Intersections xs = world.intersect(ray);
    CHECK(xs.size() == 4);
    CHECK(xs[0].t == 4.0);
    CHECK(xs[1].t == 4.5);
    CHECK(xs[2].t == 5.5);
    CHECK(xs[3].t == 6.0);
  }
  SUBCASE("Shading an intersection") {
    auto world = World::defaultWorld();
    auto ray = Ray(Tuple::point(0.0, 0.0, -5.0), Tuple::vector(0.0, 0.0, 1.0));
    auto shape = world.objects[0];
    auto intersection = Intersection(4.0, shape);
    auto computations = intersection.prepareComputations(ray);
    auto color = world.shadeHit(computations, 4);
    CHECK(color == Color(0.38066, 0.47583, 0.2855));
  }
  SUBCASE("Shading an intersection from the inside") {
    auto world = World::defaultWorld();
    world.light = PointLight(Tuple::point(0.0, 0.25, 0.0), Color(1.0, 1.0, 1.0));
    auto ray = Ray(Tuple::point(0.0, 0.0, 0.0), Tuple::vector(0.0, 0.0, 1.0));
    auto shape = world.objects[1];
    auto intersection = Intersection(.5, shape);
    auto computations = intersection.prepareComputations(ray);
    auto color = world.shadeHit(computations, 4);
    CHECK(color == Color(0.90498, 0.90498, 0.90498));
  }
  SUBCASE("Shading an intersection in shadow") {
    auto world = World();
    world.light = PointLight(Tuple::point(0.0, 0.0, 10.0), Color(1.0, 1.0, 1.0));
    auto sphere = Sphere::create();
    world.add(sphere);
    auto sphere2 = Sphere::create();
    sphere2->transform = Matrix::translation(0.0, 0.0, 10.0);
    world.add(sphere2);
    auto ray = Ray(Tuple::point(0.0, 0.0, 5.0), Tuple::vector(0.0, 0.0, 1.0));
    auto intersection = Intersection(4.0, sphere2);
    auto computations = intersection.prepareComputations(ray);
    auto color = world.shadeHit(computations, 4);
    CHECK(color == Color(0.1, 0.1, 0.1));
  }
  SUBCASE("The color when a ray misses") {
    auto world = World::defaultWorld();
    auto ray = Ray(Tuple::point(0.0, 0.0, -5.0), Tuple::vector(0.0, 1.0, 0.0));
    auto color = world.colorAt(ray, 4);
    CHECK(color == Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The color when a ray hits") {
    auto world = World::defaultWorld();
    auto ray = Ray(Tuple::point(0.0, 0.0, -5.0), Tuple::vector(0.0, 0.0, 1.0));
    auto color = world.colorAt(ray, 4);
    CHECK(color == Color(0.38066, 0.47583, 0.2855));
  }
  SUBCASE("The color with an intersection behind the ray") {
    auto world = World::defaultWorld();
    world.objects[0]->material.ambient = 1.0;
    world.objects[1]->material.ambient = 1.0;
    auto ray = Ray(Tuple::point(0.0, 0.0, 0.75), Tuple::vector(0.0, 0.0, -1.0));
    auto color = world.colorAt(ray, 4);
    CHECK(color == world.objects[1]->material.color);
  }
  SUBCASE("There is no shadow when nothing is collinear with the point and light") {
    auto world = World::defaultWorld();
    auto point = Tuple::point(0.0, 10.0, 0.0);
    CHECK(world.isShadowed(point) == false);
  }
  SUBCASE("The shadow when an object is between the point and the light") {
    auto world = World::defaultWorld();
    auto point = Tuple::point(10.0, -10.0, 10.0);
    CHECK(world.isShadowed(point) == true);
  }
  SUBCASE("There is no shadow when an object is behind the light") {
    auto world = World::defaultWorld();
    auto point = Tuple::point(-20.0, 20.0, -20.0);
    CHECK(world.isShadowed(point) == false);
  }
  SUBCASE("There is no shadow when the object is behind the point") {
    auto world = World::defaultWorld();
    auto point = Tuple::point(-2.0, 0.0, -2.0);
    CHECK(world.isShadowed(point) == false);
  }
  SUBCASE("There is no shadow when the object does not cast shadows") {
    auto world = World::defaultWorld();
    world.objects[0]->material.castShadow = false;
    auto point = Tuple::point(10.0, -10.0, 10.0);
    CHECK(world.isShadowed(point) == false);
  }
}
TEST_CASE("Reflection") {
  using namespace raytracerchallenge;
  SUBCASE("Refectivity for the default material") {
    Material material;
    CHECK(material.reflective == 0.0);
  }
  SUBCASE("Precomputing the reflection vector") {
    auto shape = Plane::create();
    auto r = Ray({0.0, 1.0, -1.0, 1.0}, {0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0});
    auto i = Intersection(sqrt(2.0), shape);
    auto comps = i.prepareComputations(r);
    CHECK(comps.reflectionVector == Tuple::vector(0.0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));
  }
  SUBCASE("The reflected color for a nonreflective material") {
    auto w = World::defaultWorld();
    auto r = Ray({0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto shape = w.objects[1];
    shape->material.ambient = 1.0;
    auto i = Intersection(1.0, shape);
    auto comps = i.prepareComputations(r);
    auto color = w.reflectedColorAt(comps, 4);
    CHECK(color == Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The reflected color for a reflective material") {
    auto w = World::defaultWorld();
    auto shape = Plane::create();
    shape->material.reflective = 0.5;
    shape->transform = shape->transform.translated(0.0, -1.0, 0.0);
    w.add(shape);
    auto r = Ray({0.0, 0.0, -3.0, 1.0}, {0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0});
    auto i = Intersection(sqrt(2.0), shape);
    auto comps = i.prepareComputations(r);
    auto color = w.reflectedColorAt(comps, 4);
    CHECK(color == Color(0.19032, 0.2379, 0.14274));
  }
  SUBCASE("Shading a hit for a reflective material") {
    auto w = World::defaultWorld();
    auto shape = Plane::create();
    shape->material.reflective = 0.5;
    shape->transform = shape->transform.translated(0.0, -1.0, 0.0);
    w.add(shape);
    auto r = Ray({0.0, 0.0, -3.0, 1.0}, {0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0});
    auto i = Intersection(sqrt(2.0), shape);
    auto comps = i.prepareComputations(r);
    auto color = w.shadeHit(comps, 4);
    CHECK(color == Color(0.87677, 0.92436, 0.82918));
  }
  SUBCASE("Coloring mutually reflective surfaces") {
    auto w = World::defaultWorld();
    w.light = PointLight({0.0, 0.0, 0.0, 1.0}, {1.0, 1.0, 1.0});
    auto lower = Plane::create();
    lower->material.reflective = 1.0;
    lower->transform = lower->transform.translated(0.0, -1.0, 0.0);
    w.add(lower);
    auto upper = Plane::create();
    upper->material.reflective = 1.0;
    upper->transform = upper->transform.translated(0.0, 1.0, 0.0);
    w.add(upper);
    auto r = Ray({0.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 0.0});
    w.colorAt(r, 4);
  }
  SUBCASE("The reflected color for a reflective material at maximum recursion depth") {
    auto w = World::defaultWorld();
    auto shape = Plane::create();
    shape->material.reflective = 0.5;
    shape->transform = shape->transform.translated(0.0, -1.0, 0.0);
    w.add(shape);
    auto r = Ray({0.0, 0.0, -3.0, 1.0}, {0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0});
    auto i = Intersection(sqrt(2.0), shape);
    auto comps = i.prepareComputations(r);
    auto color = w.reflectedColorAt(comps, 0);
    CHECK(color == Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The refracted color with an opaque surface") {
    auto w = World::defaultWorld();
    auto shape = w.objects[0];
    auto r = Ray({0.0, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = Intersections({{4.0, shape}, {6.0, shape}});
    auto comps = xs[0].prepareComputations(r, xs);
    CHECK(w.refractedColorAt(comps, 5) == Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The refracted color at the maximum recursive depth") {
    auto w = World::defaultWorld();
    auto shape = w.objects[0];
    shape->material.refractiveIndex = 1.5;
    shape->material.transparency = 1.0;
    auto r = Ray({0.0, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = Intersections({{4.0, shape}, {6.0, shape}});
    auto comps = xs[0].prepareComputations(r, xs);
    CHECK(w.refractedColorAt(comps, 0) == Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The refracted color under total internal reflection") {
    auto w = World::defaultWorld();
    auto shape = w.objects[0];
    shape->material.refractiveIndex = 1.5;
    shape->material.transparency = 1.0;
    auto r = Ray({0.0, 0.0, sqrt(2.0 / 2.0), 1.0}, {0.0, 1.0, 0.0, 0.0});
    auto xs = Intersections({{-sqrt(2.0) / 2.0, shape}, {sqrt(2.0) / 2.0, shape}});
    auto comps = xs[1].prepareComputations(r, xs);
    CHECK(w.refractedColorAt(comps, 5) == Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The refracted color with a refracted ray") {
    class TestPattern : public Pattern {
    public:
      [[nodiscard]] Color colorAt(std::shared_ptr<Shape> shape, Tuple point) const override {
        (void)shape;
        return {point.x, point.y, point.z};
      }
    };
    auto w = World::defaultWorld();
    auto a = w.objects[0];
    a->material.ambient = 1.0;
    auto pattern = TestPattern();
    a->material.pattern = &pattern;
    auto b = w.objects[1];
    b->material.transparency = 1.0;
    b->material.refractiveIndex = 1.5;
    auto r = Ray({0.0, 0.0, 0.1, 1.0}, {0.0, 1.0, 0.0, 0.0});
    auto xs = Intersections({
        {-0.9899, a},
        {-0.4899, b},
        {0.4899, b},
        {0.9899, a},
    });
    auto comps = xs[2].prepareComputations(r, xs);
    CHECK(w.refractedColorAt(comps, 5) == Color(0.0, 0.99888, 0.04725));
  }
  SUBCASE("Shading a hit on a transparent material") {
    auto w = World::defaultWorld();
    auto floor = Plane::create();
    floor->transform = Matrix::translation(0.0, -1.0, 0.0);
    floor->material.refractiveIndex = 1.5;
    floor->material.transparency = 0.5;
    w.add(floor);
    auto ball = Sphere::create();
    ball->material.color = Color(1.0, 0.0, 0.0);
    ball->material.ambient = 0.5;
    ball->transform = Matrix::translation(0.0, -3.5, -0.5);
    w.add(ball);
    auto r = Ray({0.0, 0.0, -3.0, 1.0}, {0.0, -sqrt(2.0) / 2, sqrt(2.0) / 2, 0.0});
    auto xs = Intersections({{sqrt(2.0), floor}});
    auto comps = xs[0].prepareComputations(r, xs);
    auto color = w.shadeHit(comps, 5);
    CHECK(color == Color(0.93642, 0.68642, 0.68642));
  }
  SUBCASE("Shading a hit on a reflective, transparent material") {
    auto w = World::defaultWorld();
    auto floor = Plane::create();
    floor->transform = Matrix::translation(0.0, -1.0, 0.0);
    floor->material.refractiveIndex = 1.5;
    floor->material.reflective = 0.5;
    floor->material.transparency = 0.5;
    w.add(floor);
    auto ball = Sphere::create();
    ball->material.color = Color(1.0, 0.0, 0.0);
    ball->material.ambient = 0.5;
    ball->transform = Matrix::translation(0.0, -3.5, -0.5);
    w.add(ball);
    auto r = Ray({0.0, 0.0, -3.0, 1.0}, {0.0, -sqrt(2.0) / 2, sqrt(2.0) / 2, 0.0});
    auto xs = Intersections({{sqrt(2.0), floor}});
    auto comps = xs[0].prepareComputations(r, xs);
    auto color = w.shadeHit(comps, 5);
    CHECK(color == Color(0.93391, 0.69643, 0.69243));
  }
}