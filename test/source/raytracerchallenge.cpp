#include <doctest/doctest.h>
#include <raytracerchallenge/raytracerchallenge.h>

#include <cmath>
#include <sstream>
#include <string>

TEST_CASE("Tuples") {
  using namespace raytracerchallenge;
  SUBCASE("The default tuple initializes with all values set to zero") {
    RayTracerChallenge::Tuple tuple;
    CHECK(tuple.x == 0.0f);
    CHECK(tuple.y == 0.0f);
    CHECK(tuple.z == 0.0f);
    CHECK(tuple.w == 0.0f);
  }
  SUBCASE("A tuple with w=0.1 is a point") {
    RayTracerChallenge::Tuple tuple = RayTracerChallenge::Tuple(4.3f, -4.2f, 3.1f, 1.0f);
    CHECK(tuple.x == 4.3f);
    CHECK(tuple.y == -4.2f);
    CHECK(tuple.z == 3.1f);
    CHECK(tuple.w == 1.0f);
    CHECK(tuple.isVector() == false);
  }
  SUBCASE("A tuple with w=0.0 is a vector") {
    RayTracerChallenge::Tuple tuple = RayTracerChallenge::Tuple(4.3f, -4.2f, 3.1f, 0.0f);
    CHECK(tuple.x == 4.3f);
    CHECK(tuple.y == -4.2f);
    CHECK(tuple.z == 3.1f);
    CHECK(tuple.w == 0.0f);
    CHECK(tuple.isVector() == true);
  }
  SUBCASE("point() creates tuples with w=1") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(4, -4, 3);
    CHECK(point == RayTracerChallenge::Tuple(4, -4, 3, 1.0));
  }
  SUBCASE("vector() creates tuples with w=0") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::vector(4, -4, 3);
    CHECK(point == RayTracerChallenge::Tuple(4, -4, 3, 0.0f));
  }
  SUBCASE("Adding two tuples") {
    RayTracerChallenge::Tuple t1 = RayTracerChallenge::Tuple(3, -2, 5, 1);
    RayTracerChallenge::Tuple t2 = RayTracerChallenge::Tuple(-2, 3, 1, 0);
    RayTracerChallenge::Tuple t3 = t1 + t2;
    CHECK(t3 == RayTracerChallenge::Tuple(1, 1, 6, 1));
  }
  SUBCASE("Subtracting two points") {
    RayTracerChallenge::Tuple p1 = RayTracerChallenge::Tuple::point(3, 2, 1);
    RayTracerChallenge::Tuple p2 = RayTracerChallenge::Tuple::point(5, 6, 7);
    RayTracerChallenge::Tuple p3 = p1 - p2;
    CHECK(p3 == RayTracerChallenge::Tuple::vector(-2, -4, -6));
  }
  SUBCASE("Subtracting a vector from a point") {
    RayTracerChallenge::Tuple p = RayTracerChallenge::Tuple::point(3, 2, 1);
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(5, 6, 7);
    CHECK(p - v == RayTracerChallenge::Tuple::point(-2, -4, -6));
  }
  SUBCASE("Subtracting two vectors") {
    RayTracerChallenge::Tuple v1 = RayTracerChallenge::Tuple::vector(3, 2, 1);
    RayTracerChallenge::Tuple v2 = RayTracerChallenge::Tuple::vector(5, 6, 7);
    CHECK(v1 - v2 == RayTracerChallenge::Tuple::vector(-2, -4, -6));
  }
  SUBCASE("Subtracting a vector from the zero vector") {
    RayTracerChallenge::Tuple zero = RayTracerChallenge::Tuple::vector(0, 0, 0);
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(1, -2, 3);
    CHECK(zero - v == RayTracerChallenge::Tuple::vector(-1, 2, -3));
  }
  SUBCASE("Negating a tuple") {
    RayTracerChallenge::Tuple t = RayTracerChallenge::Tuple(1, -2, 3, -4);
    CHECK(-t == RayTracerChallenge::Tuple(-1, 2, -3, 4));
  }
  SUBCASE("Multiplying a tuple by a scalar") {
    RayTracerChallenge::Tuple t = RayTracerChallenge::Tuple(1, -2, 3, -4);
    CHECK(t * 3.5 == RayTracerChallenge::Tuple(3.5, -7, 10.5, -14));
  }
  SUBCASE("Multiplying a tuple by a fraction") {
    RayTracerChallenge::Tuple t = RayTracerChallenge::Tuple(1, -2, 3, -4);
    CHECK(t * 0.5 == RayTracerChallenge::Tuple(0.5, -1, 1.5, -2));
  }
  SUBCASE("Dividing a tuple by a scalar") {
    RayTracerChallenge::Tuple t = RayTracerChallenge::Tuple(1, -2, 3, -4);
    CHECK(t / 2 == RayTracerChallenge::Tuple(0.5, -1, 1.5, -2));
  }
  SUBCASE("Computing the magnitude of a vector(1, 0, 0)") {
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(1, 0, 0);
    CHECK(v.magnitude() == 1);
  }
  SUBCASE("Computing the magnitude of a vector(0, 1, 0)") {
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(0, 1, 0);
    CHECK(v.magnitude() == 1);
  }
  SUBCASE("Computing the magnitude of a vector(0, 0, 1)") {
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(0, 0, 1);
    CHECK(v.magnitude() == 1);
  }
  SUBCASE("Computing the magnitude of a vector(1, 2, 3)") {
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(1, 2, 3);
    CHECK(abs(v.magnitude() - sqrtf(14)) < 0.000001);
  }
  SUBCASE("Computing the magnitude of a vector(-1, -2, -3)") {
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(1, 2, 3);
    CHECK(abs(v.magnitude() - sqrtf(14)) < 0.000001);
  }
  SUBCASE("Normalizing vector(4, 0, 0) gives (1, 0, 0)") {
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(4, 0, 0);
    CHECK(v.normalize() == RayTracerChallenge::Tuple::vector(1, 0, 0));
  }
  SUBCASE("Normalizing vector(1, 2, 3)") {
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(1, 2, 3);
    CHECK(v.normalize()
          == RayTracerChallenge::Tuple::vector(1 / sqrtf(14), 2 / sqrtf(14), 3 / sqrtf(14)));
  }
  SUBCASE("The magnitude of a normalized vector") {
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(1, 2, 3);
    RayTracerChallenge::Tuple v1 = v.normalize();
    CHECK(abs(v1.magnitude() - 1) < 0.000001);
  }
  SUBCASE("The dot product of two tuples") {
    RayTracerChallenge::Tuple a = RayTracerChallenge::Tuple::vector(1, 2, 3);
    RayTracerChallenge::Tuple b = RayTracerChallenge::Tuple::vector(2, 3, 4);
    CHECK(a.dot(b) == 20);
  }
  SUBCASE("The cross product of two vectors") {
    RayTracerChallenge::Tuple a = RayTracerChallenge::Tuple::vector(1, 2, 3);
    RayTracerChallenge::Tuple b = RayTracerChallenge::Tuple::vector(2, 3, 4);
    CHECK(a.cross(b) == RayTracerChallenge::Tuple::vector(-1, 2, -1));
    CHECK(b.cross(a) == RayTracerChallenge::Tuple::vector(1, -2, 1));
  }
}

TEST_CASE("Colors") {
  using namespace raytracerchallenge;
  SUBCASE("Colors are (red, green, blue) tuples") {
    RayTracerChallenge::Color c = RayTracerChallenge::Color(-0.5f, 0.4f, 1.7f);
    CHECK(c.red == -0.5);
    CHECK(c.green == 0.4f);
    CHECK(c.blue == 1.7f);
  }
  SUBCASE("Default Color is black") {
    RayTracerChallenge::Color c = RayTracerChallenge::Color();
    CHECK(c.red == 0.0f);
    CHECK(c.green == 0.0f);
    CHECK(c.blue == 0.0f);
  }
  SUBCASE("Adding colors") {
    RayTracerChallenge::Color c1 = RayTracerChallenge::Color(0.9f, 0.6f, 0.75f);
    RayTracerChallenge::Color c2 = RayTracerChallenge::Color(0.7f, 0.1f, 0.25f);
    CHECK(c1 + c2 == RayTracerChallenge::Color(1.6f, 0.7f, 1.0f));
  }
  SUBCASE("Subtracting colors") {
    RayTracerChallenge::Color c1 = RayTracerChallenge::Color(0.9f, 0.6f, 0.75f);
    RayTracerChallenge::Color c2 = RayTracerChallenge::Color(0.7f, 0.1f, 0.25f);
    CHECK(c1 - c2 == RayTracerChallenge::Color(0.2f, 0.5f, 0.5f));
  }
  SUBCASE("Multiplying a color by a scalar") {
    RayTracerChallenge::Color c = RayTracerChallenge::Color(0.2f, 0.3f, 0.4f);
    CHECK(c * 2 == RayTracerChallenge::Color(0.4f, 0.6f, 0.8f));
  }
  SUBCASE("Multiplying colors") {
    RayTracerChallenge::Color c1 = RayTracerChallenge::Color(1.0f, 0.2f, 0.4f);
    RayTracerChallenge::Color c2 = RayTracerChallenge::Color(0.9f, 1.0f, 0.1f);
    CHECK(c1 * c2 == RayTracerChallenge::Color(0.9f, 0.2f, 0.04f));
  }
}

TEST_CASE("Canvas") {
  using namespace raytracerchallenge;
  SUBCASE("Creating a canvas") {
    RayTracerChallenge::Canvas c = RayTracerChallenge::Canvas(10, 20);
    CHECK(c.width == 10);
    CHECK(c.height == 20);
    for (int x = 0; x < 10; x++) {
      for (int y = 0; y < 20; y++) {
        CHECK(c.pixelAt(x, y) == RayTracerChallenge::Color(0.0f, 0.0f, 0.0f));
      }
    }
  }
  SUBCASE("Writing pixels to a Canvas") {
    RayTracerChallenge::Canvas c = RayTracerChallenge::Canvas(10, 20);
    RayTracerChallenge::Color red = RayTracerChallenge::Color(1.0f, 0.0f, 0.0f);
    c.writePixel(2, 3, red);
    CHECK(c.pixelAt(2, 3) == red);
  }
  SUBCASE("Constructing the PPM header") {
    RayTracerChallenge::Canvas c = RayTracerChallenge::Canvas(5, 3);
    std::string ppm = c.toPortablePixmap();
    std::istringstream stream(ppm);
    std::string line;
    std::vector<std::string> items;
    while (std::getline(stream, line)) {
      items.push_back(line);
    }
    CHECK(items[0] == "P3");
    CHECK(items[1] == "5 3");
    CHECK(items[2] == "255");
  }
  SUBCASE("Constructing the PPM pixel data") {
    RayTracerChallenge::Canvas c = RayTracerChallenge::Canvas(5, 3);
    RayTracerChallenge::Color c1(1.5f, 0.0f, 0.0f);
    RayTracerChallenge::Color c2(0.0f, 0.5f, 0.0f);
    RayTracerChallenge::Color c3(-0.5f, 0.0f, 1.0f);
    c.writePixel(0, 0, c1);
    c.writePixel(2, 1, c2);
    c.writePixel(4, 2, c3);
    std::string ppm = c.toPortablePixmap();
    std::istringstream stream(ppm);
    std::string line;
    std::vector<std::string> items;
    while (std::getline(stream, line)) {
      items.push_back(line);
    }
    CHECK(items[3] == "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
    CHECK(items[4] == "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0");
    CHECK(items[5] == "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255");
  }
  SUBCASE("Splitting long lines in PPM files") {
    RayTracerChallenge::Canvas c = RayTracerChallenge::Canvas(10, 2);
    RayTracerChallenge::Color c1(1.0f, 0.8f, 0.6f);
    for (int y = 0; y < c.height; y++) {
      for (int x = 0; x < c.width; x++) {
        c.writePixel(x, y, c1);
      }
    }
    std::string ppm = c.toPortablePixmap();
    std::istringstream stream(ppm);
    std::string line;
    std::vector<std::string> items;
    while (std::getline(stream, line)) {
      items.push_back(line);
    }
    CHECK(items[3] == "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
    CHECK(items[4] == "153 255 204 153 255 204 153 255 204 153 255 204 153");
    CHECK(items[5] == "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
    CHECK(items[6] == "153 255 204 153 255 204 153 255 204 153 255 204 153");
  }
  SUBCASE("PPM files are terminated by a newline character") {
    RayTracerChallenge::Canvas c = RayTracerChallenge::Canvas(5, 3);
    std::string ppm = c.toPortablePixmap();
    CHECK(ppm.back() == '\n');
  }
}