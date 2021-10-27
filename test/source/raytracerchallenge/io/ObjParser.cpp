#include <doctest/doctest.h>
#include <raytracerchallenge/base/Tuple.h>
#include <raytracerchallenge/io/ObjParser.h>
#include <raytracerchallenge/shapes/SmoothTriangle.h>
#include <raytracerchallenge/shapes/Triangle.h>

#include <memory>

using namespace raytracerchallenge;

TEST_CASE("OBJ file parser") {
  SUBCASE("An OBJ file with vertex data") {
    auto f = std::stringstream(
        ""
        "v -1 1 0\n"
        "v -1.0000 0.5000 0.0000\n"
        "v 1 0 0\n"
        "v 1 1 0\n"
        "");
    auto res = ObjParser::parse(f);
    CHECK(res.vertices[1] == Tuple::point(-1.0, 1.0, 0.0));
    CHECK(res.vertices[2] == Tuple::point(-1.0, 0.5, 0.0));
    CHECK(res.vertices[3] == Tuple::point(1.0, 0.0, 0.0));
    CHECK(res.vertices[4] == Tuple::point(1.0, 1.0, 0.0));
  }
  SUBCASE("Parsing triangle faces") {
    auto f = std::stringstream(
        ""
        "v -1 1 0\n"
        "v -1 0 0\n"
        "v 1 0 0\n"
        "v 1 1 0\n"
        "\n"
        "f 1 2 3\n"
        "f 1 3 4\n"
        "");
    auto res = ObjParser::parse(f);
    auto g = res.defaultGroup;
    auto objects = std::dynamic_pointer_cast<Group>(g)->objects;
    auto t1 = std::dynamic_pointer_cast<Triangle>(objects[0]);
    auto t2 = std::dynamic_pointer_cast<Triangle>(objects[1]);
    CHECK(t1->p1 == res.vertices[1]);
    CHECK(t1->p2 == res.vertices[2]);
    CHECK(t1->p3 == res.vertices[3]);
    CHECK(t2->p1 == res.vertices[1]);
    CHECK(t2->p2 == res.vertices[3]);
    CHECK(t2->p3 == res.vertices[4]);
  }
  SUBCASE("Triangulating polygons") {
    auto f = std::stringstream(
        ""
        "v -1 1 0\n"
        "v -1 0 0\n"
        "v 1 0 0\n"
        "v 1 1 0\n"
        "v 0 2 0\n"
        "\n"
        "f 1 2 3 4 5\n"
        "");
    auto res = ObjParser::parse(f);
    auto g = res.defaultGroup;
    auto objects = std::dynamic_pointer_cast<Group>(g)->objects;
    auto t1 = std::dynamic_pointer_cast<Triangle>(objects[0]);
    auto t2 = std::dynamic_pointer_cast<Triangle>(objects[1]);
    auto t3 = std::dynamic_pointer_cast<Triangle>(objects[2]);
    CHECK(t1->p1 == res.vertices[1]);
    CHECK(t1->p2 == res.vertices[2]);
    CHECK(t1->p3 == res.vertices[3]);
    CHECK(t2->p1 == res.vertices[1]);
    CHECK(t2->p2 == res.vertices[3]);
    CHECK(t2->p3 == res.vertices[4]);
    CHECK(t3->p1 == res.vertices[1]);
    CHECK(t3->p2 == res.vertices[4]);
    CHECK(t3->p3 == res.vertices[5]);
  }
  SUBCASE("Triangles in groups") {
    auto f = std::stringstream(
        ""
        "v -1 1 0\n"
        "v -1 0 0\n"
        "v 1 0 0\n"
        "v 1 1 0\n"
        "g FirstGroup\n"
        "f 1 2 3\n"
        "g SecondGroup\n"
        "f 1 3 4\n"
        "");
    auto res = ObjParser::parse(f);
    auto g1 = res.groups["FirstGroup"];
    auto g2 = res.groups["SecondGroup"];
    auto t1ptr = std::dynamic_pointer_cast<Group>(g1)->objects[0];
    auto t2ptr = std::dynamic_pointer_cast<Group>(g2)->objects[0];
    auto t1 = std::dynamic_pointer_cast<Triangle>(t1ptr);
    auto t2 = std::dynamic_pointer_cast<Triangle>(t2ptr);
    CHECK(t1->p1 == res.vertices[1]);
    CHECK(t1->p2 == res.vertices[2]);
    CHECK(t1->p3 == res.vertices[3]);
    CHECK(t2->p1 == res.vertices[1]);
    CHECK(t2->p2 == res.vertices[3]);
    CHECK(t2->p3 == res.vertices[4]);
  }
  SUBCASE("Converting an OBJ file to a group") {
    auto f = std::stringstream(
        ""
        "v -1 1 0\n"
        "v -1 0 0\n"
        "v 1 0 0\n"
        "v 1 1 0\n"
        "g FirstGroup\n"
        "f 1 2 3\n"
        "g SecondGroup\n"
        "f 1 3 4\n"
        "");
    auto res = ObjParser::parse(f);
    auto shape = res.getObjects();
    auto shapeGroup = std::dynamic_pointer_cast<Group>(shape);
    auto g1 = res.groups["FirstGroup"];
    auto g2 = res.groups["SecondGroup"];
    auto g3 = res.groups["Default"];
    CHECK(std::find(shapeGroup->objects.begin(), shapeGroup->objects.end(), g1)
          != shapeGroup->objects.end());
    CHECK(std::find(shapeGroup->objects.begin(), shapeGroup->objects.end(), g2)
          != shapeGroup->objects.end());
    CHECK(std::find(shapeGroup->objects.begin(), shapeGroup->objects.end(), g3)
          != shapeGroup->objects.end());
  }
  SUBCASE("Vertex normal records") {
    auto f = std::stringstream(
        ""
        "vn 0 0 1\n"
        "vn 0.707 0 -0.707\n"
        "vn 1 2 3\n"
        "");
    auto res = ObjParser::parse(f);
    CHECK(res.normals[1] == Tuple::vector(0.0, 0.0, 1.0));
    CHECK(res.normals[2] == Tuple::vector(0.707, 0.0, -0.707));
    CHECK(res.normals[3] == Tuple::vector(1.0, 2.0, 3.0));
  }
  SUBCASE("Faces with normals") {
    auto f = std::stringstream(
        ""
        "v 0 1 0\n"
        "v -1 0 0\n"
        "v 1 0 0\n"
        "vn -1 0 0\n"
        "vn 1 0 0\n"
        "vn 0 1 0\n"
        "f 1//3 2//1 3//2\n"
        "f 1/0/3 2/102/1 2/14/2\n"
        "");
    auto res = ObjParser::parse(f);
    auto g = res.defaultGroup;
    auto objects = std::dynamic_pointer_cast<Group>(g)->objects;
    auto t1 = std::dynamic_pointer_cast<SmoothTriangle>(objects[0]);
    auto t2 = std::dynamic_pointer_cast<SmoothTriangle>(objects[1]);
    CHECK(t1->p1 == res.vertices[1]);
    CHECK(t1->p2 == res.vertices[2]);
    CHECK(t1->p3 == res.vertices[3]);
    CHECK(t1->n1 == res.normals[3]);
    CHECK(t1->n2 == res.normals[1]);
    CHECK(t1->n3 == res.normals[2]);
    CHECK(*t1 == *t2);
  }
}