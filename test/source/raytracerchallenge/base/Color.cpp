#include <doctest/doctest.h>
#include <raytracerchallenge/base/Color.h>

using namespace raytracerchallenge;

TEST_CASE("Colors") {
  SUBCASE("Colors are (red, green, blue) tuples") {
    Color c = Color(-0.5, 0.4, 1.7);
    CHECK(c.red == -0.5);
    CHECK(c.green == 0.4);
    CHECK(c.blue == 1.7);
  }
  SUBCASE("Default Color is black") {
    Color c = Color();
    CHECK(c.red == 0.0);
    CHECK(c.green == 0.0);
    CHECK(c.blue == 0.0);
  }
  SUBCASE("Adding colors") {
    Color c1 = Color(0.9, 0.6, 0.75);
    Color c2 = Color(0.7, 0.1, 0.25);
    CHECK(c1 + c2 == Color(1.6, 0.7, 1.0));
  }
  SUBCASE("Subtracting colors") {
    Color c1 = Color(0.9, 0.6, 0.75);
    Color c2 = Color(0.7, 0.1, 0.25);
    CHECK(c1 - c2 == Color(0.2, 0.5, 0.5));
  }
  SUBCASE("Multiplying a color by a scalar") {
    Color c = Color(0.2, 0.3, 0.4);
    CHECK(c * 2 == Color(0.4, 0.6, 0.8));
  }
  SUBCASE("Multiplying colors") {
    Color c1 = Color(1.0, 0.2, 0.4);
    Color c2 = Color(0.9, 1.0, 0.1);
    CHECK(c1 * c2 == Color(0.9, 0.2, 0.04));
  }
}