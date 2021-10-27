#include <doctest/doctest.h>
#include <raytracerchallenge/base/Canvas.h>

#include <iostream>
#include <sstream>

using namespace raytracerchallenge;

TEST_CASE("Canvas") {
  SUBCASE("Creating a canvas") {
    Canvas c = Canvas(10, 20);
    CHECK(c.width == 10);
    CHECK(c.height == 20);
    for (int x = 0; x < 10; x++) {
      for (int y = 0; y < 20; y++) {
        CHECK(c.pixelAt(x, y) == Color(0.0, 0.0, 0.0));
      }
    }
  }
  SUBCASE("Writing pixels to a Canvas") {
    Canvas c = Canvas(10, 20);
    Color red = Color(1.0, 0.0, 0.0);
    c.writePixel(2, 3, red);
    CHECK(c.pixelAt(2, 3) == red);
  }
  SUBCASE("Constructing the PPM header") {
    Canvas c = Canvas(5, 3);
    std::string ppm = c.toPortablePixmap();
    std::istringstream stream(ppm);
    std::string line;
    std::vector<std::string> items;
    while (getline(stream, line)) {
      items.push_back(line);
    }
    CHECK(items[0] == "P3");
    CHECK(items[1] == "5 3");
    CHECK(items[2] == "255");
  }
  SUBCASE("Constructing the PPM pixel data") {
    Canvas c = Canvas(5, 3);
    Color c1(1.5, 0.0, 0.0);
    Color c2(0.0, 0.5, 0.0);
    Color c3(-0.5, 0.0, 1.0);
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
    Canvas c = Canvas(10, 2);
    Color c1(1.0, 0.8, 0.6);
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
    Canvas c = Canvas(5, 3);
    std::string ppm = c.toPortablePixmap();
    CHECK(ppm.back() == '\n');
  }
}
