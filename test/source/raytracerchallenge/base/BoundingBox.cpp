#define _USE_MATH_DEFINES
#include <doctest/doctest.h>
#include <raytracerchallenge/base/BoundingBox.h>
#include <raytracerchallenge/shapes/Sphere.h>

#include <cmath>

using namespace raytracerchallenge;

TEST_CASE("Bounding boxes") {
  SUBCASE("Creating a bounding box with volume") {
    auto b = BoundingBox({-1.0, -2.0, -3.0, 1.0}, {3.0, 2.0, 1.0, 1.0});
    CHECK(b.min == Tuple(-1.0, -2.0, -3.0, 1.0));
    CHECK(b.max == Tuple(3.0, 2.0, 1.0, 1.0));
  }
  SUBCASE("Adding points to an empty bounding box") {
    auto b = BoundingBox();
    b.add({-5.0, 2.0, 0.0, 1.0});
    b.add({7.0, 0.0, -3.0, 1.0});
    CHECK(b.min == Tuple(-5.0, -0.0, -3.0, 1.0));
    CHECK(b.max == Tuple(7.0, 2.0, 0.0, 1.0));
  }
  SUBCASE("Adding one bounding box to another") {
    auto box1 = BoundingBox({-5.0, -2.0, 0.0, 1.0}, {7.0, 4.0, 4.0, 1.0});
    auto box2 = BoundingBox({8.0, -7.0, -2.0, 1.0}, {14.0, 2.0, 8.0, 1.0});
    box1.add(box2);
    CHECK(box1.min == Tuple(-5.0, -7.0, -2.0, 1.0));
    CHECK(box1.max == Tuple(14.0, 4.0, 8.0, 1.0));
  }
  SUBCASE("Checking to see if a box contains a given point") {
    auto box = BoundingBox({5.0, -2.0, 0.0, 1.0}, {11.0, 4.0, 7.0, 1.0});
    CHECK(box.contains({5.0, -2.0, 0.0, 1.0}));
    CHECK(box.contains({11.0, 4.0, 7.0, 1.0}));
    CHECK(box.contains({8.0, 1.0, 3.0, 1.0}));
    CHECK(!box.contains({3.0, 0.0, 3.0, 1.0}));
    CHECK(!box.contains({8.0, -4.0, 3.0, 1.0}));
    CHECK(!box.contains({8.0, 1.0, -1.0, 1.0}));
    CHECK(!box.contains({13.0, 1.0, 3.0, 1.0}));
    CHECK(!box.contains({8.0, 5.0, 3.0, 1.0}));
    CHECK(!box.contains({8.0, 1.0, 8.0, 1.0}));
  }
  SUBCASE("Checking to see if box contains another box") {
    auto box1 = BoundingBox({5.0, -2.0, 0.0, 1.0}, {11.0, 4.0, 7.0, 1.0});
    CHECK(box1.contains({{5.0, -2.0, 0.0, 1.0}, {11.0, 4.0, 7.0, 1.0}}));
    CHECK(box1.contains({{6.0, -1.0, 1.0, 1.0}, {10.0, 3.0, 6.0, 1.0}}));
    CHECK(!box1.contains({{4.0, -3.0, 0 - 10, 1.0}, {10.0, 3.0, 6.0, 1.0}}));
    CHECK(!box1.contains({{6.0, -1.0, 1.0, 1.0}, {12.0, 5.0, 8.0, 1.0}}));
  }
  SUBCASE("Transforming bounding boxes") {
    auto box = BoundingBox({-1.0, -1.0, -1.0, 1.0}, {1.0, 1.0, 1.0, 1.0});
    auto matrix = Matrix::rotationX(M_PI / 4.0) * Matrix::rotationY(M_PI / 4.0);
    auto box2 = box.transform(matrix);
    CHECK(box2.min == Tuple(-1.4142, -1.7071, -1.7071, 1.0));
    CHECK(box2.max == Tuple(1.4142, 1.7071, 1.7071, 1.0));
  }
  SUBCASE("Querying a shape's bounding box in its parent's space") {
    auto shape = Sphere::create();
    shape->transform = shape->transform.scaled(0.5, 2.0, 4.0).translated(1.0, -3.0, 5.0);
    auto box = shape->parentSpaceBounds();
    CHECK(box.min == Tuple(0.5, -5.0, 1.0, 1.0));
    CHECK(box.max == Tuple(1.5, -1.0, 9.0, 1.0));
  }
  SUBCASE("Intersecting a ray with a bounding box at the origin") {
    auto box = BoundingBox({-1.0, -1.0, -1.0, 1.0}, {1.0, 1.0, 1.0, 1.0});
    CHECK(box.intersects({{5.0, 0.5, 0.0, 1.0}, {-1.0, 0.0, 0.0, 0.0}}));
    CHECK(box.intersects({{-5.0, 0.5, 0.0, 1.0}, {1.0, 0.0, 0.0, 0.0}}));
    CHECK(box.intersects({{0.5, 5.0, 0.0, 1.0}, {0.0, -1.0, 0.0, 0.0}}));
    CHECK(box.intersects({{0.5, -5.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 0.0}}));
    CHECK(box.intersects({{0.5, 0.0, 5.0, 1.0}, {0.0, 0.0, -1.0, 0.0}}));
    CHECK(box.intersects({{0.5, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0}}));
    CHECK(box.intersects({{0.0, 0.5, 0.0, 1.0}, {0.0, 0.0, 1.0, 0.0}}));
    CHECK(!box.intersects({{-2.0, 0.0, 0.0, 1.0}, {2.0, 4.0, 6.0, 0.0}}));
    CHECK(!box.intersects({{0.0, -2.0, 0.0, 1.0}, {6.0, 2.0, 4.0, 0.0}}));
    CHECK(!box.intersects({{0.0, 0.0, -2.0, 1.0}, {4.0, 6.0, 2.0, 0.0}}));
    CHECK(!box.intersects({{2.0, 0.0, 2.0, 1.0}, {0.0, 0.0, -1.0, 0.0}}));
    CHECK(!box.intersects({{0.0, 2.0, 2.0, 1.0}, {0.0, -1.0, 0.0, 0.0}}));
    CHECK(!box.intersects({{2.0, 2.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 0.0}}));
  }
  SUBCASE("Intersecting a ray with a non-cubic bounding box") {
    auto box = BoundingBox({5.0, -2.0, 0.0, 1.0}, {11.0, 4.0, 7.0, 1.0});
    CHECK(box.intersects({{15.0, 1.0, 2.0, 1.0}, {-1.0, 0.0, 0.0, 0.0}}));
    CHECK(box.intersects({{-5.0, -1.0, 4.0, 1.0}, {1.0, 0.0, 0.0, 0.0}}));
    CHECK(box.intersects({{7.0, 6.0, 5.0, 1.0}, {0.0, -1.0, 0.0, 0.0}}));
    CHECK(box.intersects({{9.0, -5.0, 6.0, 1.0}, {0.0, 1.0, 0.0, 0.0}}));
    CHECK(box.intersects({{8.0, 2.0, 12.0, 1.0}, {0.0, 0.0, -1.0, 0.0}}));
    CHECK(box.intersects({{6.0, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0}}));
    CHECK(box.intersects({{8.0, 1.0, 3.5, 1.0}, {0.0, 0.0, 1.0, 0.0}}));
    CHECK(!box.intersects({{9.0, -1.0, -8.0, 1.0}, {2.0, 4.0, 6.0, 0.0}}));
    CHECK(!box.intersects({{8.0, 3.0, -4.0, 1.0}, {6.0, 2.0, 4.0, 0.0}}));
    CHECK(!box.intersects({{9.0, -1.0, -2.0, 1.0}, {4.0, 6.0, 2.0, 0.0}}));
    CHECK(!box.intersects({{4.0, 0.0, 9.0, 1.0}, {0.0, 0.0, -1.0, 0.0}}));
    CHECK(!box.intersects({{8.0, 6.0, -1.0, 1.0}, {0.0, -1.0, 0.0, 0.0}}));
    CHECK(!box.intersects({{12.0, 5.0, 4.0, 1.0}, {-1.0, 0.0, 0.0, 0.0}}));
  }
  SUBCASE("Splitting a perfect cube") {
    auto box = BoundingBox({-1.0, -4.0, -5.0, 1.0}, {9.0, 6.0, 5.0, 1.0});
    auto boxes = box.split();
    auto left = boxes[0];
    auto right = boxes[1];
    CHECK(left.min == Tuple(-1.0, -4.0, -5.0, 1.0));
    CHECK(left.max == Tuple(4.0, 6.0, 5.0, 1.0));
    CHECK(right.min == Tuple(4.0, -4.0, -5.0, 1.0));
    CHECK(right.max == Tuple(9.0, 6.0, 5.0, 1.0));
  }
  SUBCASE("Splitting an x-wide box") {
    auto box = BoundingBox({-1.0, -2.0, -3.0, 1.0}, {9.0, 5.5, 3.0, 1.0});
    auto boxes = box.split();
    auto left = boxes[0];
    auto right = boxes[1];
    CHECK(left.min == Tuple(-1.0, -2.0, -3.0, 1.0));
    CHECK(left.max == Tuple(4.0, 5.5, 3.0, 1.0));
    CHECK(right.min == Tuple(4.0, -2.0, -3.0, 1.0));
    CHECK(right.max == Tuple(9.0, 5.5, 3.0, 1.0));
  }
  SUBCASE("Splitting an y-wide box") {
    auto box = BoundingBox({-1.0, -2.0, -3.0, 1.0}, {5.0, 8.0, 3.0, 1.0});
    auto boxes = box.split();
    auto left = boxes[0];
    auto right = boxes[1];
    CHECK(left.min == Tuple(-1.0, -2.0, -3.0, 1.0));
    CHECK(left.max == Tuple(5.0, 3.0, 3.0, 1.0));
    CHECK(right.min == Tuple(-1.0, 3.0, -3.0, 1.0));
    CHECK(right.max == Tuple(5.0, 8.0, 3.0, 1.0));
  }
  SUBCASE("Splitting an y-wide box") {
    auto box = BoundingBox({-1.0, -2.0, -3.0, 1.0}, {5.0, 8.0, 3.0, 1.0});
    auto boxes = box.split();
    auto left = boxes[0];
    auto right = boxes[1];
    CHECK(left.min == Tuple(-1.0, -2.0, -3.0, 1.0));
    CHECK(left.max == Tuple(5.0, 3.0, 3.0, 1.0));
    CHECK(right.min == Tuple(-1.0, 3.0, -3.0, 1.0));
    CHECK(right.max == Tuple(5.0, 8.0, 3.0, 1.0));
  }
  SUBCASE("Splitting an z-wide box") {
    auto box = BoundingBox({-1.0, -2.0, -3.0, 1.0}, {5.0, 3.0, 7.0, 1.0});
    auto boxes = box.split();
    auto left = boxes[0];
    auto right = boxes[1];
    CHECK(left.min == Tuple(-1.0, -2.0, -3.0, 1.0));
    CHECK(left.max == Tuple(5.0, 3.0, 2.0, 1.0));
    CHECK(right.min == Tuple(-1.0, -2.0, 2.0, 1.0));
    CHECK(right.max == Tuple(5.0, 3.0, 7.0, 1.0));
  }
}