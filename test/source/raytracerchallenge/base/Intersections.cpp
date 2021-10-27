
#include <doctest/doctest.h>
#include <raytracerchallenge/base/Computations.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/base/Tuple.h>
#include <raytracerchallenge/shapes/GlassSphere.h>
#include <raytracerchallenge/shapes/Sphere.h>

using namespace raytracerchallenge;

TEST_CASE("Ray-sphere intersections") {
  SUBCASE("An intersection encapsulates t and object") {
    auto sphere = Sphere::create();
    Intersection intersection(3.5, sphere);
    CHECK(intersection.t == 3.5);
    CHECK(*intersection.object == *sphere);
  }
  SUBCASE("Aggregating intersections") {
    auto sphere = Sphere::create();
    Intersection intersection1(1.0, sphere);
    Intersection intersection2(2.0, sphere);
    std::vector<Intersection> intersections{intersection1, intersection2};
    CHECK(intersections.size() == 2);
    CHECK(*intersections[0].object == *sphere);
    CHECK(*intersections[1].object == *sphere);
  }
  SUBCASE("A ray intersects a sphere at two points") {
    Tuple origin = Tuple::point(0.0, 0.0, -5.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    auto sphere = Sphere::create();
    Ray ray(origin, direction);
    Intersections intersections = sphere->intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == 4.0);
    CHECK(intersections[1].t == 6.0);
    CHECK(*sphere == *intersections[0].object);
    CHECK(*sphere == *intersections[1].object);
  }
  SUBCASE("A ray intersects a sphere at a tangent") {
    Tuple origin = Tuple::point(0.0, 1.0, -5.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    auto sphere = Sphere::create();
    Ray ray(origin, direction);
    Intersections intersections = sphere->intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == 5.0);
    CHECK(intersections[1].t == 5.0);
    CHECK(*sphere == *intersections[0].object);
    CHECK(*sphere == *intersections[1].object);
  }
  SUBCASE("A ray originates inside a sphere") {
    Tuple origin = Tuple::point(0.0, 0.0, 0.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    auto sphere = Sphere::create();
    Ray ray(origin, direction);
    Intersections intersections = sphere->intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == -1.0);
    CHECK(intersections[1].t == 1.0);
    CHECK(*sphere == *intersections[0].object);
    CHECK(*sphere == *intersections[1].object);
  }
  SUBCASE("A sphere is behind a ray") {
    Tuple origin = Tuple::point(0.0, 0.0, 5.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    auto sphere = Sphere::create();
    Ray ray(origin, direction);
    Intersections intersections = sphere->intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == -6.0);
    CHECK(intersections[1].t == -4.0);
    CHECK(*sphere == *intersections[0].object);
    CHECK(*sphere == *intersections[1].object);
  }
  SUBCASE("The hit, when all intersections have positive t") {
    auto sphere = Sphere::create();
    Intersection intersection1(1.0, sphere);
    Intersection intersection2(1.0, sphere);
    Intersections xs({intersection1, intersection2});
    CHECK(xs.hit() == intersection1);
  }
  SUBCASE("The hit, when some intersections have negative t") {
    auto sphere = Sphere::create();
    Intersection intersection1(-1.0, sphere);
    Intersection intersection2(1.0, sphere);
    Intersections xs({intersection1, intersection2});
    CHECK(xs.hit() == intersection2);
  }
  SUBCASE("The hit, when all intersections have negative t") {
    auto sphere = Sphere::create();
    Intersection intersection1(-2.0, sphere);
    Intersection intersection2(-1.0, sphere);
    Intersections xs({intersection1, intersection2});
    CHECK(xs.hit().has_value() == false);
  }
  SUBCASE("The hit is always the lowest nonnegative intersection") {
    auto sphere = Sphere::create();
    Intersection intersection1(5.0, sphere);
    Intersection intersection2(7.0, sphere);
    Intersection intersection3(-3.0, sphere);
    Intersection intersection4(2.0, sphere);
    Intersections xs({intersection1, intersection2, intersection3, intersection4});
    CHECK(xs.hit() == intersection4);
  }
  SUBCASE("Intersecting a scaled sphere with a ray") {
    Tuple origin = Tuple::point(0.0, 0.0, -5.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    Ray ray(origin, direction);
    auto sphere = Sphere::create();
    sphere->transform = Matrix::scaling(2.0, 2.0, 2.0);
    Intersections xs = sphere->intersect(ray);
    CHECK(xs.size() == 2);
    CHECK(xs[0].t == 3.0);
    CHECK(xs[1].t == 7.0);
  }
  SUBCASE("Intersecting a translated sphere with a ray") {
    Tuple origin = Tuple::point(0.0, 0.0, -5.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    Ray ray(origin, direction);
    auto sphere = Sphere::create();
    sphere->transform = Matrix::translation(5.0, 0.0, 0.0);
    Intersections xs = sphere->intersect(ray);
    CHECK(xs.size() == 0);
  }
  SUBCASE("Precomputing the state of an intersection") {
    Tuple origin = Tuple::point(0.0, 0.0, -5.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    Ray ray(origin, direction);
    auto sphere = Sphere::create();
    Intersection intersection(4.0, sphere);
    Computations computations = intersection.prepareComputations(ray);
    CHECK(computations.t == intersection.t);
    CHECK(computations.object->is(*intersection.object));
    CHECK(computations.point == Tuple::point(0.0, 0.0, -1.0));
    CHECK(computations.eyeVector == Tuple::vector(0.0, 0.0, -1.0));
    CHECK(computations.normalVector == Tuple::vector(0.0, 0.0, -1.0));
  }
  SUBCASE("The hit, when an intersection occurs on the outside") {
    Tuple origin = Tuple::point(0.0, 0.0, -5.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    Ray ray(origin, direction);
    auto sphere = Sphere::create();
    Intersection intersection(4.0, sphere);
    Computations computations = intersection.prepareComputations(ray);
    CHECK(computations.inside == false);
  }
  SUBCASE("The hit, when an intersection occurs on the inside") {
    Tuple origin = Tuple::point(0.0, 0.0, 0.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    Ray ray(origin, direction);
    auto sphere = Sphere::create();
    Intersection intersection(1.0, sphere);
    Computations computations = intersection.prepareComputations(ray);
    CHECK(computations.point == Tuple::point(0.0, 0.0, 1.0));
    CHECK(computations.eyeVector == Tuple::vector(0.0, 0.0, -1.0));
    CHECK(computations.normalVector == Tuple::vector(0.0, 0.0, -1.0));
    CHECK(computations.inside == true);
  }
  SUBCASE("The hit should offset the point") {
    Tuple origin = Tuple::point(0.0, 0.0, -5.0);
    Tuple direction = Tuple::vector(0.0, 0.0, 1.0);
    Ray ray(origin, direction);
    auto sphere = Sphere::create();
    sphere->transform = Matrix::translation(0.0, 0.0, 1.0);
    Intersection intersection(5.0, sphere);
    Computations computations = intersection.prepareComputations(ray);
    CHECK(computations.overPoint.z < 0.0001 / 2.0);
    CHECK(computations.point.z > computations.overPoint.z);
  }
  SUBCASE("Finding n1 and n2 at various intersections") {
    auto a = GlassSphere::create();
    a->transform = a->transform.scaled(2.0, 2.0, 2.0);
    auto b = GlassSphere::create();
    b->transform = b->transform.translated(0.0, 0.0, -0.25);
    b->material.refractiveIndex = 2.0;
    auto c = GlassSphere::create();
    c->transform = c->transform.translated(0.0, 0.0, 0.25);
    c->material.refractiveIndex = 2.5;
    auto r = Ray({0.0, 0.0, -4.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = Intersections({
        {2.0, a},
        {2.75, b},
        {3.25, c},
        {4.75, b},
        {5.25, c},
        {6.0, a},
    });
    CHECK(xs[0].prepareComputations(r, xs).n1 == 1.0);
    CHECK(xs[0].prepareComputations(r, xs).n2 == 1.5);
    CHECK(xs[1].prepareComputations(r, xs).n1 == 1.5);
    CHECK(xs[1].prepareComputations(r, xs).n2 == 2.0);
    CHECK(xs[2].prepareComputations(r, xs).n1 == 2.0);
    CHECK(xs[2].prepareComputations(r, xs).n2 == 2.5);
    CHECK(xs[3].prepareComputations(r, xs).n1 == 2.5);
    CHECK(xs[3].prepareComputations(r, xs).n2 == 2.5);
    CHECK(xs[4].prepareComputations(r, xs).n1 == 2.5);
    CHECK(xs[4].prepareComputations(r, xs).n2 == 1.5);
    CHECK(xs[5].prepareComputations(r, xs).n1 == 1.5);
    CHECK(xs[5].prepareComputations(r, xs).n2 == 1.0);
  }
  SUBCASE("The under point is offset below the surface") {
    auto r = Ray({0.0, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto shape = GlassSphere::create();
    shape->transform = shape->transform.translated(0.0, 0.0, 1.0);
    auto intersection = Intersection(5.0, shape);
    auto comps = intersection.prepareComputations(r);
    CHECK(comps.underPoint.z > 0.0001 / 2.0);
    CHECK(comps.point.z < comps.underPoint.z);
  }
}