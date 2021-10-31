#include <doctest/doctest.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/CSG.h>
#include <raytracerchallenge/shapes/Cube.h>
#include <raytracerchallenge/shapes/Group.h>
#include <raytracerchallenge/shapes/Sphere.h>

using namespace raytracerchallenge;

TEST_CASE("Constructive Solid Geometry") {
  SUBCASE("A CSG is created with an operation and two shapes") {
    auto s1 = Sphere::create();
    auto s2 = Cube::create();
    auto c = CSG::create(s1, s2);
    auto cc = std::dynamic_pointer_cast<CSG>(c);
    CHECK(cc->operation == CSG::Union);
    CHECK(cc->left == s1);
    CHECK(cc->right == s2);
    CHECK(s1->parent == c);
    CHECK(s2->parent == c);
  }
  SUBCASE("Filtering intersections for a Union") {
    auto s1 = Sphere::create();
    auto s2 = Cube::create();
    auto c = CSG::create(s1, s2, CSG::Union);
    auto cc = std::dynamic_pointer_cast<CSG>(c);
    auto xs = Intersections({Intersection(1.0, s1), Intersection(2.0, s2), Intersection(3.0, s1),
                             Intersection(4.0, s2)});
    auto result = cc->filterIntersections(xs);
    CHECK(result.size() == 2);
    CHECK(result.intersections[0] == Intersection(1, s1));
    CHECK(result.intersections[1] == Intersection(4, s2));
  }
  SUBCASE("Filtering intersections for an Intersection") {
    auto s1 = Sphere::create();
    auto s2 = Cube::create();
    auto c = CSG::create(s1, s2, CSG::Intersection);
    auto cc = std::dynamic_pointer_cast<CSG>(c);
    auto xs = Intersections({Intersection(1.0, s1), Intersection(2.0, s2), Intersection(3.0, s1),
                             Intersection(4.0, s2)});
    auto result = cc->filterIntersections(xs);
    CHECK(result.size() == 2);
    CHECK(result.intersections[0] == Intersection(2.0, s2));
    CHECK(result.intersections[1] == Intersection(3.0, s1));
  }
  SUBCASE("Filtering intersections for a Difference") {
    auto s1 = Sphere::create();
    auto s2 = Cube::create();
    auto c = CSG::create(s1, s2, CSG::Difference);
    auto cc = std::dynamic_pointer_cast<CSG>(c);
    auto xs = Intersections({Intersection(1.0, s1), Intersection(2.0, s2), Intersection(3.0, s1),
                             Intersection(4.0, s2)});
    auto result = cc->filterIntersections(xs);
    CHECK(result.size() == 2);
    CHECK(result.intersections[0] == Intersection(1.0, s1));
    CHECK(result.intersections[1] == Intersection(2.0, s2));
  }
  SUBCASE("A ray misses a CSG object") {
    auto s1 = Sphere::create();
    auto s2 = Cube::create();
    auto c = CSG::create(s1, s2, CSG::Union);
    auto r = Ray({0.0, 2.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = c->localIntersect(r);
    CHECK(xs.size() == 0);
  }
  SUBCASE("A ray hits a CSG object") {
    auto s1 = Sphere::create();
    auto s2 = Sphere::create();
    s2->transform = s2->transform.translated(0.0, 0.0, 0.5);
    auto c = CSG::create(s1, s2, CSG::Union);
    auto r = Ray({0.0, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = c->localIntersect(r);
    CHECK(xs.size() == 2);
    CHECK(xs[0].t == 4);
    CHECK(xs[0].object == s1);
    CHECK(xs[1].t == 6.5);
    CHECK(xs[1].object == s2);
  }
  SUBCASE("A CSG has a bounding box which contains its children") {
    auto left = Sphere::create();
    auto right = Sphere::create();
    right->transform = right->transform.translated(2.0, 3.0, 4.0);
    auto shape = CSG::create(left, right, CSG::Difference);
    auto box = shape->bounds();
    CHECK(box.min == Tuple::point(-1.0, -1.0, -1.0));
    CHECK(box.max == Tuple::point(3.0, 4.0, 5.0));
  }
  SUBCASE("Subdividing a CSG shape subdivides its children") {
    auto s1 = Sphere::create();
    s1->transform = s1->transform.translated(-1.5, 0.0, 0.0);
    auto s2 = Sphere::create();
    s2->transform = s2->transform.translated(1.5, 0.0, 0.0);
    auto left = Group();
    left.add(s1);
    left.add(s2);
    auto s3 = Sphere::create();
    s3->transform = s3->transform.translated(0.0, 0.0, -1.5);
    auto s4 = Sphere::create();
    s4->transform = s4->transform.translated(0.0, 0.0, 1.5);
    auto right = Group();
    right.add(s3);
    right.add(s4);
    auto shape = CSG::create(left.sharedPtr, right.sharedPtr, CSG::Difference);
    shape->divide(1);
    CHECK(left.objects[0]->includes(*s1));
    CHECK(left.objects[1]->includes(*s2));
    CHECK(right.objects[0]->includes(*s3));
    CHECK(right.objects[1]->includes(*s4));
  }
}