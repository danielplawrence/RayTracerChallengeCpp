#include <doctest/doctest.h>
#include <raytracerchallenge/shapes/Cylinder.h>
#include <raytracerchallenge/shapes/Group.h>
#include <raytracerchallenge/shapes/Sphere.h>

using namespace raytracerchallenge;

TEST_CASE("Groups") {
  SUBCASE("Creating a new group") {
    auto g = Group::create();
    CHECK(g->transform == Matrix::identity(4));
  }
  SUBCASE("Adding a child to a group") {
    auto gr = Group::create();
    auto s = Sphere::create();
    auto g = std::dynamic_pointer_cast<Group>(gr);
    g->add(s);
    CHECK(g->objects[0] == s);
    CHECK(s->parent == gr);
  }
  SUBCASE("Intersecting a ray with an empty group") {
    auto g = Group::create();
    auto r = Ray({0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = g->localIntersect(r);
    CHECK(xs.size() == 0);
  }
  SUBCASE("Intersecting a ray with a non-empty group") {
    auto g = Group::create();
    auto s1 = Sphere::create();
    auto s2 = Sphere::create();
    auto s3 = Sphere::create();
    s2->transform = s2->transform.translated(0.0, 0.0, -3.0);
    s3->transform = s3->transform.translated(5.0, 0.0, 0.0);
    std::dynamic_pointer_cast<Group>(g)->add(s1);
    std::dynamic_pointer_cast<Group>(g)->add(s2);
    std::dynamic_pointer_cast<Group>(g)->add(s3);
    auto r = Ray({0.0, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = g->localIntersect(r);
    CHECK(xs[0].object == s2);
    CHECK(xs[1].object == s2);
    CHECK(xs[2].object == s1);
    CHECK(xs[3].object == s1);
  }
  SUBCASE("Intersecting a transformed group") {
    auto g = Group::create();
    g->transform = g->transform.scaled(2.0, 2.0, 2.0);
    auto s = Sphere::create();
    s->transform = s->transform.translation(5.0, 0.0, 0.0);
    std::dynamic_pointer_cast<Group>(g)->add(s);
    auto r = Ray({10.0, 0.0, -10.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = g->intersect(r);
    CHECK(xs.size() == 2);
  }
  SUBCASE("Converting a point from world to object space") {
    auto g1 = Group::create();
    g1->transform = g1->transform.rotatedY(M_PI / 2.0);
    auto g2 = Group::create();
    g2->transform = g2->transform.scaled(2.0, 2.0, 2.0);
    std::dynamic_pointer_cast<Group>(g1)->add(g2);
    auto s = Sphere::create();
    s->transform = s->transform.translation(5.0, 0.0, 0.0);
    std::dynamic_pointer_cast<Group>(g2)->add(s);
    auto p = s->worldToObject({-2.0, 0.0, -10.0, 1.0});
    CHECK(p == Tuple({0.0, 0.0, -1.0, 1.0}));
  }
  SUBCASE("Converting a normal from object to world space") {
    auto g1 = Group::create();
    g1->transform = g1->transform.rotatedY(M_PI / 2.0);
    auto g2 = Group::create();
    g2->transform = g2->transform.scaled(1.0, 2.0, 3.0);
    std::dynamic_pointer_cast<Group>(g1)->add(g2);
    auto s = Sphere::create();
    s->transform = s->transform.translation(5.0, 0.0, 0.0);
    std::dynamic_pointer_cast<Group>(g2)->add(s);
    auto n = s->normalToWorld({sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, 0.0});
    CHECK(n == Tuple(0.2857, 0.4286, -0.8571, 0.0));
  }
  SUBCASE("Finding the normal on a child object") {
    auto g1 = Group::create();
    g1->transform = g1->transform.rotatedY(M_PI / 2.0);
    auto g2 = Group::create();
    g2->transform = g2->transform.scaled(1.0, 2.0, 3.0);
    std::dynamic_pointer_cast<Group>(g1)->add(g2);
    auto s = Sphere::create();
    s->transform = s->transform.translation(5.0, 0.0, 0.0);
    std::dynamic_pointer_cast<Group>(g2)->add(s);
    auto n = s->normalAt({1.7321, 1.1547, -5.5774, 1.0}, {});
    CHECK(n == Tuple(0.2857, 0.4286, -0.8571, 0.0));
  }
  SUBCASE("A group has a bounding box which contains its children") {
    auto s = Sphere::create();
    s->transform = s->transform.scaled(2.0, 2.0, 2.0).translated(2.0, 5.0, -3.0);
    auto cyl = Cylinder::create(-2.0, 2.0, true);
    cyl->transform = cyl->transform.scaled(0.5, 1.0, 0.5).translated(-4.0, -1.0, 4.0);
    auto shape = Group();
    shape.add(s);
    shape.add(cyl);
    auto box = shape.bounds();
    CHECK(box.min == Tuple(-4.5, -3.0, -5.0, 1.0));
    CHECK(box.max == Tuple(4.0, 7.0, 4.5, 1.0));
  }
}