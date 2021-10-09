#define _USE_MATH_DEFINES
#include <doctest/doctest.h>
#include <raytracerchallenge/raytracerchallenge.h>

#include <cmath>
#include <memory>
#include <sstream>

using namespace raytracerchallenge;

double INF = INFINITY;

TEST_CASE("Tuples") {
  using namespace raytracerchallenge;
  SUBCASE("The default tuple initializes with all values set to zero") {
    RayTracerChallenge::Tuple tuple;
    CHECK(tuple.x == 0.0);
    CHECK(tuple.y == 0.0);
    CHECK(tuple.z == 0.0);
    CHECK(tuple.w == 0.0);
  }
  SUBCASE("A tuple with w=0.1 is a point") {
    RayTracerChallenge::Tuple tuple = RayTracerChallenge::Tuple(4.3, -4.2, 3.1, 1.0);
    CHECK(tuple.x == 4.3);
    CHECK(tuple.y == -4.2);
    CHECK(tuple.z == 3.1);
    CHECK(tuple.w == 1.0);
    CHECK(tuple.isVector() == false);
  }
  SUBCASE("A tuple with w=0.0 is a vector") {
    RayTracerChallenge::Tuple tuple = RayTracerChallenge::Tuple(4.3, -4.2, 3.1, 0.0);
    CHECK(tuple.x == 4.3);
    CHECK(tuple.y == -4.2);
    CHECK(tuple.z == 3.1);
    CHECK(tuple.w == 0.0);
    CHECK(tuple.isVector() == true);
  }
  SUBCASE("point() creates tuples with w=1") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(4, -4, 3);
    CHECK(point == RayTracerChallenge::Tuple(4, -4, 3, 1.0));
  }
  SUBCASE("vector() creates tuples with w=0") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::vector(4, -4, 3);
    CHECK(point == RayTracerChallenge::Tuple(4, -4, 3, 0.0));
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
    CHECK(abs(v.magnitude() - sqrt(14)) < 0.000001);
  }
  SUBCASE("Computing the magnitude of a vector(-1, -2, -3)") {
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(1, 2, 3);
    CHECK(abs(v.magnitude() - sqrt(14)) < 0.000001);
  }
  SUBCASE("Normalizing vector(4, 0, 0) gives (1, 0, 0)") {
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(4, 0, 0);
    CHECK(v.normalize() == RayTracerChallenge::Tuple::vector(1, 0, 0));
  }
  SUBCASE("Normalizing vector(1, 2, 3)") {
    RayTracerChallenge::Tuple v = RayTracerChallenge::Tuple::vector(1, 2, 3);
    CHECK(v.normalize()
          == RayTracerChallenge::Tuple::vector(1 / sqrt(14), 2 / sqrt(14), 3 / sqrt(14)));
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
    RayTracerChallenge::Color c = RayTracerChallenge::Color(-0.5, 0.4, 1.7);
    CHECK(c.red == -0.5);
    CHECK(c.green == 0.4);
    CHECK(c.blue == 1.7);
  }
  SUBCASE("Default Color is black") {
    RayTracerChallenge::Color c = RayTracerChallenge::Color();
    CHECK(c.red == 0.0);
    CHECK(c.green == 0.0);
    CHECK(c.blue == 0.0);
  }
  SUBCASE("Adding colors") {
    RayTracerChallenge::Color c1 = RayTracerChallenge::Color(0.9, 0.6, 0.75);
    RayTracerChallenge::Color c2 = RayTracerChallenge::Color(0.7, 0.1, 0.25);
    CHECK(c1 + c2 == RayTracerChallenge::Color(1.6, 0.7, 1.0));
  }
  SUBCASE("Subtracting colors") {
    RayTracerChallenge::Color c1 = RayTracerChallenge::Color(0.9, 0.6, 0.75);
    RayTracerChallenge::Color c2 = RayTracerChallenge::Color(0.7, 0.1, 0.25);
    CHECK(c1 - c2 == RayTracerChallenge::Color(0.2, 0.5, 0.5));
  }
  SUBCASE("Multiplying a color by a scalar") {
    RayTracerChallenge::Color c = RayTracerChallenge::Color(0.2, 0.3, 0.4);
    CHECK(c * 2 == RayTracerChallenge::Color(0.4, 0.6, 0.8));
  }
  SUBCASE("Multiplying colors") {
    RayTracerChallenge::Color c1 = RayTracerChallenge::Color(1.0, 0.2, 0.4);
    RayTracerChallenge::Color c2 = RayTracerChallenge::Color(0.9, 1.0, 0.1);
    CHECK(c1 * c2 == RayTracerChallenge::Color(0.9, 0.2, 0.04));
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
        CHECK(c.pixelAt(x, y) == RayTracerChallenge::Color(0.0, 0.0, 0.0));
      }
    }
  }
  SUBCASE("Writing pixels to a Canvas") {
    RayTracerChallenge::Canvas c = RayTracerChallenge::Canvas(10, 20);
    RayTracerChallenge::Color red = RayTracerChallenge::Color(1.0, 0.0, 0.0);
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
    RayTracerChallenge::Color c1(1.5, 0.0, 0.0);
    RayTracerChallenge::Color c2(0.0, 0.5, 0.0);
    RayTracerChallenge::Color c3(-0.5, 0.0, 1.0);
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
    RayTracerChallenge::Color c1(1.0, 0.8, 0.6);
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

TEST_CASE("Matrices") {
  using namespace raytracerchallenge;
  SUBCASE("Constructing and inspecting a 4x4 matrix") {
    std::vector<std::vector<double>> vect{
        {1.0, 2.0, 3.0, 4.0},
        {5.5, 6.5, 7.5, 8.5},
        {9.0, 10.0, 11.0, 12.0},
        {13.5, 14.5, 15.5, 16.5},
    };
    RayTracerChallenge::Matrix matrix(4, 4, vect);
    CHECK(matrix.m(0, 0) == 1.0);
    CHECK(matrix.m(0, 3) == 4.0);
    CHECK(matrix.m(1, 0) == 5.5);
    CHECK(matrix.m(1, 2) == 7.5);
    CHECK(matrix.m(2, 2) == 11.0);
    CHECK(matrix.m(3, 0) == 13.5);
    CHECK(matrix.m(3, 2) == 15.5);
  }
  SUBCASE("A 2x2 matrix ought to be representable") {
    std::vector<std::vector<double>> vect{{-3.0, 5.0}, {1.0, -2.0}};
    RayTracerChallenge::Matrix matrix(2, 2, vect);
    CHECK(matrix.m(0, 0) == -3.0);
    CHECK(matrix.m(0, 1) == 5.0);
    CHECK(matrix.m(1, 0) == 1.0);
    CHECK(matrix.m(1, 1) == -2.0);
  }
  SUBCASE("A 3x3 matrix ought to be representable") {
    std::vector<std::vector<double>> vect{{-3.0, 5.0, 0.0}, {1.0, -2.0, -7.0}, {0.0, 1.0, 1.0}};
    RayTracerChallenge::Matrix matrix(3, 3, vect);
    CHECK(matrix.m(0, 0) == -3.0);
    CHECK(matrix.m(1, 1) == -2.0);
    CHECK(matrix.m(2, 2) == 1.0);
  }
  SUBCASE("Matrix equality with identical matrices") {
    std::vector<std::vector<double>> vect1{
        {1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    std::vector<std::vector<double>> vect2{
        {1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK((matrix2 == matrix1) == true);
  }
  SUBCASE("Matrix equality with different matrices") {
    std::vector<std::vector<double>> vect1{
        {1.0, 2.0, 3.0, 4.0}, {5.5, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    std::vector<std::vector<double>> vect2{
        {2.0, 3.0, 4.0, 5.0}, {6.0, 7.0, 8.0, 9.0}, {8.0, 7.0, 6.0, 5.0}, {4.0, 3.0, 2.0, 1.0}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK((matrix2 == matrix1) == false);
  }
  SUBCASE("Matrix inequality with different matrices") {
    std::vector<std::vector<double>> vect1{
        {1.0, 2.0, 3.0, 4.0}, {5.5, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    std::vector<std::vector<double>> vect2{
        {2.0, 3.0, 4.0, 5.0}, {6.0, 7.0, 8.0, 9.0}, {8.0, 7.0, 6.0, 5.0}, {4.0, 3.0, 2.0, 1.0}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK((matrix2 != matrix1) == true);
  }
  SUBCASE("Matrix inequality with identical matrices") {
    std::vector<std::vector<double>> vect1{
        {1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    std::vector<std::vector<double>> vect2{
        {1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK((matrix2 != matrix1) == false);
  }
  SUBCASE("Multiplying two matrices") {
    std::vector<std::vector<double>> vect1{
        {1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    std::vector<std::vector<double>> vect2{
        {-2.0, 1.0, 2.0, 3.0}, {3.0, 2.0, 1.0, -1.0}, {4.0, 3.0, 6.0, 5.0}, {1.0, 2.0, 7.0, 8.0}};
    std::vector<std::vector<double>> vect3{{20.0, 22.0, 50.0, 48.0},
                                           {44.0, 54.0, 114.0, 108.0},
                                           {40.0, 58.0, 110.0, 102.0},
                                           {16.0, 26.0, 46.0, 42.0}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    RayTracerChallenge::Matrix matrix3(4, 4, vect3);
    CHECK(matrix1 * matrix2 == matrix3);
  }
  SUBCASE("A matrix multiplied by a tuple") {
    std::vector<std::vector<double>> vect1{
        {1.0, 2.0, 3.0, 4.0}, {2.0, 4.0, 4.0, 2.0}, {8.0, 6.0, 4.0, 1.0}, {0.0, 0.0, 0.0, 1.0}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Tuple tuple(1, 2, 3, 1);
    CHECK(matrix1 * tuple == RayTracerChallenge::Tuple(18, 24, 33, 1));
  }
  SUBCASE("The identity matrix") {
    std::vector<std::vector<double>> identity{
        {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
    RayTracerChallenge::Matrix matrix1(4, 4, identity);
    RayTracerChallenge::Matrix matrix2 = RayTracerChallenge::Matrix::identity(4);
    CHECK(matrix2 == matrix1);
  }
  SUBCASE("Multiplying a matrix by the identity matrix") {
    std::vector<std::vector<double>> vect1{
        {0.0, 0.0, 2.0, 4.0}, {1.0, 2.0, 4.0, 8.0}, {2.0, 4.0, 8.0, 16.0}, {4.0, 8.0, 16.0, 32.0}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2 = RayTracerChallenge::Matrix::identity(4);
    CHECK(matrix1 * matrix2 == matrix1);
  }
  SUBCASE("Transposing a matrix") {
    std::vector<std::vector<double>> vect1{
        {0.0, 9.0, 3.0, 0.0}, {9.0, 8.0, 0.0, 8.0}, {1.0, 8.0, 5.0, 3.0}, {0.0, 0.0, 5.0, 8.0}};
    std::vector<std::vector<double>> vect2{
        {0.0, 9.0, 1.0, 0.0}, {9.0, 8.0, 8.0, 0.0}, {3.0, 0.0, 5.0, 5.0}, {0.0, 8.0, 3.0, 8.0}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK(matrix1.transposed() == matrix2);
  }
  SUBCASE("Transposing the identity matrix") {
    RayTracerChallenge::Matrix matrix1 = RayTracerChallenge::Matrix::identity(4);
    CHECK(matrix1.transposed() == matrix1);
  }
  SUBCASE("Calculating the determinant of a 2x2 matrix") {
    std::vector<std::vector<double>> vect1{
        {1.0, 5.0},
        {-3.0, 2.0},
    };
    RayTracerChallenge::Matrix matrix1(2, 2, vect1);
    CHECK(matrix1.determinant() == 17.0);
  }
  SUBCASE("Calculating the determinant of a 3x3 matrix") {
    std::vector<std::vector<double>> vect{{1.0, 2.0, 6.0}, {-5.0, 8.0, -4.0}, {2.0, 6.0, 4.0}};
    RayTracerChallenge::Matrix matrix(3, 3, vect);
    CHECK(matrix.determinant() == -196.0);
  }
  SUBCASE("Calculating the determinant of a 4x4 matrix") {
    std::vector<std::vector<double>> vect{{-2.0, -8.0, 3.0, 5.0},
                                          {-3.0, 1.0, 7.0, 3.0},
                                          {1.0, 2.0, -9.0, 6.0},
                                          {-6.0, 7.0, 7.0, -9.0}};
    RayTracerChallenge::Matrix matrix(4, 4, vect);
    CHECK(abs(matrix.determinant() - -4071.0) < 0.0001);
  }
  SUBCASE("Testing an invertible matrix for invertibility") {
    std::vector<std::vector<double>> vect{
        {6.0, 4.0, 4.0, 4.0}, {5.0, 5.0, 7.0, 6.0}, {4.0, -9.0, 3.0, -7.0}, {9.0, 1.0, 7.0, -6.0}};
    RayTracerChallenge::Matrix matrix(4, 4, vect);
    CHECK(matrix.invertible() == true);
  }
  SUBCASE("Testing an noninvertible matrix for invertibility") {
    std::vector<std::vector<double>> vect{{-4.0, 2.0, -2.0, -3.0},
                                          {9.0, 6.0, 2.0, 6.0},
                                          {0.0, -5.0, 1.0, -5.0},
                                          {0.0, 0.0, 0.0, 0.0}};
    RayTracerChallenge::Matrix matrix(4, 4, vect);
    CHECK(matrix.invertible() == false);
  }
  SUBCASE("Calculating the inverse of a matrix") {
    std::vector<std::vector<double>> vect1{{-5.0, 2.0, 6.0, -8.0},
                                           {1.0, -5.0, 1.0, 8.0},
                                           {7.0, 7.0, -6.0, -7.0},
                                           {1.0, -3.0, 7.0, 4.0}};
    std::vector<std::vector<double>> vect2{{0.21805, 0.45113, 0.24060, -0.04511},
                                           {-0.80827, -1.45677, -0.44361, 0.52068},
                                           {-0.07895, -0.22368, -0.05263, 0.19737},
                                           {-0.52256, -0.81391, -0.30075, 0.30639}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK(matrix1.inverse() == matrix2);
  }
  SUBCASE("Calculating the inverse of another matrix") {
    std::vector<std::vector<double>> vect1{{8.0, -5.0, 9.0, 2.0},
                                           {7.0, 5.0, 6.0, 1.0},
                                           {-6.0, 0.0, 9.0, 6.0},
                                           {-3.0, 0.0, -9.0, -4.0}};
    std::vector<std::vector<double>> vect2{{-0.15385, -0.15385, -0.28205, -0.53846},
                                           {-0.07692, 0.12308, 0.02564, 0.03077},
                                           {0.35897, 0.35897, 0.43590, 0.92308},
                                           {-0.69231, -0.69231, -0.76932, -1.92308}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK(matrix1.inverse() == matrix2);
  }
  SUBCASE("Calculating the inverse of a third matrix") {
    std::vector<std::vector<double>> vect1{{9.0, 3.0, 0.0, 9.0},
                                           {-5.0, -2.0, -6.0, -3.0},
                                           {-4.0, 9.0, 6.0, 4.0},
                                           {-7.0, 6.0, 6.0, 2.0}};
    std::vector<std::vector<double>> vect2{{-0.04074, -0.07778, 0.14444, -0.22222},
                                           {-0.07778, 0.03333, 0.36667, -0.33333},
                                           {-0.02901, -0.14630, -0.10926, 0.12963},
                                           {0.17778, 0.06667, -0.26667, 0.33333}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK(matrix1.inverse() == matrix2);
  }
  SUBCASE("Multiplying a product by its inverse") {
    std::vector<std::vector<double>> vect1{{3.0, -9.0, 7.0, 3.0},
                                           {3.0, -8.0, 2.0, -9.0},
                                           {-4.0, 4.0, 4.0, 1.0},
                                           {-6.0, 5.0, -1.0, 1.0}};
    std::vector<std::vector<double>> vect2{
        {8.0, 2.0, 2.0, 2.0}, {3.0, -1.0, 7.0, 0.0}, {7.0, 0.0, 5.0, 4.0}, {6.0, -2.0, 0.0, 5.0}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    RayTracerChallenge::Matrix product = matrix1 * matrix2;
    CHECK(product * matrix2.inverse() == matrix1);
  }
}
TEST_CASE("Matrix transformations") {
  using namespace raytracerchallenge;
  SUBCASE("Multiplying by a transformation matrix") {
    RayTracerChallenge::Matrix transform = RayTracerChallenge::Matrix::translation(5.0, -3.0, 2.0);
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(-3.0, 4.0, 5.0);
    CHECK(transform * point == RayTracerChallenge::Tuple::point(2.0, 1.0, 7.0));
  }
  SUBCASE("Multiplying by the inverse of a transformation matrix") {
    RayTracerChallenge::Matrix transform = RayTracerChallenge::Matrix::translation(5.0, -3.0, 2.0);
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(-3.0, 4.0, 5.0);
    CHECK(transform.inverse() * point == RayTracerChallenge::Tuple::point(-8.0, 7.0, 3.0));
  }
  SUBCASE("Translation does not affect vectors") {
    RayTracerChallenge::Matrix transform = RayTracerChallenge::Matrix::translation(5.0, -3.0, 2.0);
    RayTracerChallenge::Tuple vector = RayTracerChallenge::Tuple::vector(-3.0, 4.0, 5.0);
    CHECK(transform * vector == vector);
  }
  SUBCASE("A scaling matrix applied to a point") {
    RayTracerChallenge::Matrix transform = RayTracerChallenge::Matrix::scaling(2.0, 3.0, 4.0);
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(-4.0, 6.0, 8.0);
    CHECK(transform * point == RayTracerChallenge::Tuple::point(-8.0, 18.0, 32.0));
  }
  SUBCASE("A scaling matrix applied to a vector") {
    RayTracerChallenge::Matrix transform = RayTracerChallenge::Matrix::scaling(2.0, 3.0, 4.0);
    RayTracerChallenge::Tuple vector = RayTracerChallenge::Tuple::vector(-4.0, 6.0, 8.0);
    CHECK(transform * vector == RayTracerChallenge::Tuple::vector(-8.0, 18.0, 32.0));
  }
  SUBCASE("Multiplying by the inverse of a scaling matrix") {
    RayTracerChallenge::Matrix transform = RayTracerChallenge::Matrix::scaling(2.0, 3.0, 4.0);
    RayTracerChallenge::Tuple vector = RayTracerChallenge::Tuple::vector(-4.0, 6.0, 8.0);
    CHECK(transform.inverse() * vector == RayTracerChallenge::Tuple::vector(-2.0, 2.0, 2.0));
  }
  SUBCASE("Reflection is scaling by a negative value") {
    RayTracerChallenge::Matrix transform = RayTracerChallenge::Matrix::scaling(-1.0, 1.0, 1.0);
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == RayTracerChallenge::Tuple::point(-2.0, 3.0, 4.0));
  }
  SUBCASE("Rotating a point around the x axis") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(0.0, 1.0, 0.0);
    RayTracerChallenge::Matrix halfQuarter = RayTracerChallenge::Matrix::rotationX(M_PI / 4.0);
    RayTracerChallenge::Matrix fullQuarter = RayTracerChallenge::Matrix::rotationX(M_PI / 2.0);
    CHECK(halfQuarter * point
          == RayTracerChallenge::Tuple::point(0.0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));
    CHECK(fullQuarter * point == RayTracerChallenge::Tuple::point(0.0, 0.0, 1.0));
  }
  SUBCASE("The inverse of an x-rotation rotates in the opposite direction") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(0.0, 1.0, 0.0);
    RayTracerChallenge::Matrix halfQuarter = RayTracerChallenge::Matrix::rotationX(M_PI / 4.0);
    CHECK(halfQuarter.inverse() * point
          == RayTracerChallenge::Tuple::point(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0));
  }
  SUBCASE("Rotating a point around the y axis") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(0.0, 0.0, 1.0);
    RayTracerChallenge::Matrix halfQuarter = RayTracerChallenge::Matrix::rotationY(M_PI / 4.0);
    RayTracerChallenge::Matrix fullQuarter = RayTracerChallenge::Matrix::rotationY(M_PI / 2.0);
    CHECK(halfQuarter * point
          == RayTracerChallenge::Tuple::point(sqrt(2.0) / 2.0, 0.0, sqrt(2.0) / 2.0));
    CHECK(fullQuarter * point == RayTracerChallenge::Tuple::point(1.0, 0.0, 0.0));
  }
  SUBCASE("Rotating a point around the z axis") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(0.0, 1.0, 0.0);
    RayTracerChallenge::Matrix halfQuarter = RayTracerChallenge::Matrix::rotationZ(M_PI / 4.0);
    RayTracerChallenge::Matrix fullQuarter = RayTracerChallenge::Matrix::rotationZ(M_PI / 2.0);
    CHECK(halfQuarter * point
          == RayTracerChallenge::Tuple::point(-sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0));
    CHECK(fullQuarter * point == RayTracerChallenge::Tuple::point(-1.0, 0.0, 0.0));
  }
  SUBCASE("A shearing transformation moves x in proportion to y") {
    RayTracerChallenge::Matrix transform
        = RayTracerChallenge::Matrix::shearing(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == RayTracerChallenge::Tuple::point(5.0, 3.0, 4.0));
  }
  SUBCASE("A shearing transformation moves x in proportion to z") {
    RayTracerChallenge::Matrix transform
        = RayTracerChallenge::Matrix::shearing(0.0, 1.0, 0.0, 0.0, 0.0, 0.0);
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == RayTracerChallenge::Tuple::point(6.0, 3.0, 4.0));
  }
  SUBCASE("A shearing transformation moves y in proportion to x") {
    RayTracerChallenge::Matrix transform
        = RayTracerChallenge::Matrix::shearing(0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == RayTracerChallenge::Tuple::point(2.0, 5.0, 4.0));
  }
  SUBCASE("A shearing transformation moves y in proportion to z") {
    RayTracerChallenge::Matrix transform
        = RayTracerChallenge::Matrix::shearing(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == RayTracerChallenge::Tuple::point(2.0, 7.0, 4.0));
  }
  SUBCASE("A shearing transformation moves z in proportion to x") {
    RayTracerChallenge::Matrix transform
        = RayTracerChallenge::Matrix::shearing(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == RayTracerChallenge::Tuple::point(2.0, 3.0, 6.0));
  }
  SUBCASE("A shearing transformation moves z in proportion to y") {
    RayTracerChallenge::Matrix transform
        = RayTracerChallenge::Matrix::shearing(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == RayTracerChallenge::Tuple::point(2.0, 3.0, 7.0));
  }
  SUBCASE("Individual transformations are applied in sequence") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(1.0, 0.0, 1.0);
    RayTracerChallenge::Matrix transformA = RayTracerChallenge::Matrix::rotationX(M_PI / 2.0);
    RayTracerChallenge::Matrix transformB = RayTracerChallenge::Matrix::scaling(5.0, 5.0, 5.0);
    RayTracerChallenge::Matrix transformC = RayTracerChallenge::Matrix::translation(10.0, 5.0, 7.0);
    RayTracerChallenge::Tuple point2 = transformA * point;
    CHECK(point2 == RayTracerChallenge::Tuple::point(1.0, -1.0, 0.0));
    RayTracerChallenge::Tuple point3 = transformB * point2;
    CHECK(point3 == RayTracerChallenge::Tuple::point(5.0, -5.0, 0.0));
    RayTracerChallenge::Tuple point4 = transformC * point3;
    CHECK(point4 == RayTracerChallenge::Tuple::point(15.0, 0.0, 7.0));
  }
  SUBCASE("Chained transformations must be applied in reverse order") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(1.0, 0.0, 1.0);
    RayTracerChallenge::Matrix transformA = RayTracerChallenge::Matrix::rotationX(M_PI / 2.0);
    RayTracerChallenge::Matrix transformB = RayTracerChallenge::Matrix::scaling(5.0, 5.0, 5.0);
    RayTracerChallenge::Matrix transformC = RayTracerChallenge::Matrix::translation(10.0, 5.0, 7.0);
    RayTracerChallenge::Matrix transforms = transformC * transformB * transformA;
    CHECK(transforms * point == RayTracerChallenge::Tuple::point(15.0, 0.0, 7.0));
  }
  SUBCASE("Fluent transformations") {
    RayTracerChallenge::Tuple point = RayTracerChallenge::Tuple::point(1.0, 0.0, 1.0);
    RayTracerChallenge::Matrix transform = RayTracerChallenge::Matrix::identity(4)
                                               .rotatedX(M_PI / 2.0)
                                               .rotatedY(M_PI / 2.0)
                                               .rotatedZ(M_PI / 2.0)
                                               .scaled(5.0, 5.0, 5.0)
                                               .translated(10.0, 5.0, 7.0)
                                               .sheared(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    CHECK(transform * point == RayTracerChallenge::Tuple::point(20.0, 5.0, 2.0));
  }
  SUBCASE("The transformation matrix for the default orientation") {
    auto from = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto to = RayTracerChallenge::Tuple::point(0.0, 0.0, -1.0);
    auto up = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
    auto transformation = RayTracerChallenge::Matrix::view(from, to, up);
    CHECK(transformation == RayTracerChallenge::Matrix::identity(4));
  }
  SUBCASE("A view transformation matrix looking in positive z direction") {
    auto from = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto to = RayTracerChallenge::Tuple::point(0.0, 0.0, 1.0);
    auto up = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
    auto transformation = RayTracerChallenge::Matrix::view(from, to, up);
    CHECK(transformation == RayTracerChallenge::Matrix::scaling(-1.0, 1.0, -1.0));
  }
  SUBCASE("The view transformation moves the world") {
    auto from = RayTracerChallenge::Tuple::point(0.0, 0.0, 8.0);
    auto to = RayTracerChallenge::Tuple::point(0.0, 0.0, 1.0);
    auto up = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
    auto transformation = RayTracerChallenge::Matrix::view(from, to, up);
    CHECK(transformation == RayTracerChallenge::Matrix::translation(0.0, 0.0, -8.0));
  }
  SUBCASE("An arbitrary view transformation") {
    auto from = RayTracerChallenge::Tuple::point(1.0, 3.0, 2.0);
    auto to = RayTracerChallenge::Tuple::point(4.0, -2.0, 8.0);
    auto up = RayTracerChallenge::Tuple::vector(1.0, 1.0, 0.0);
    auto transformation = RayTracerChallenge::Matrix::view(from, to, up);
    RayTracerChallenge::Matrix matrix(4, 4,
                                      {{-0.50709, 0.50709, 0.67612, -2.36643},
                                       {0.76772, 0.60609, 0.12122, -2.82843},
                                       {-0.35857, 0.59761, -0.71714, 0.00000},
                                       {0.00000, 0.00000, 0.00000, 1.00000}});
    CHECK(transformation == matrix);
  }
}
TEST_CASE("Spheres") {
  using namespace raytracerchallenge;
  SUBCASE("Spheres have unique identifiers") {
    auto sphere1 = RayTracerChallenge::Sphere::create();
    auto sphere2 = RayTracerChallenge::Sphere::create();
    CHECK(sphere1->id != sphere2->id);
  }
  SUBCASE("A Sphere has a default material") {
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Material material;
    CHECK(sphere->material == material);
  }
  SUBCASE("A Sphere may be assigned a material") {
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Material material;
    material.ambient = 1.0;
    sphere->material = material;
    CHECK(sphere->material == material);
  }
}
TEST_CASE("Ray-sphere intersections") {
  using namespace raytracerchallenge;
  SUBCASE("Creating and querying a ray") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(1.0, 2.0, 3.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(4.0, 5.0, 6.0);
    RayTracerChallenge::Ray ray(origin, direction);
    CHECK(ray.origin == origin);
    CHECK(ray.direction == direction);
  }
  SUBCASE("Computing a point from a distance") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(2.0, 3.0, 4.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(1.0, 0.0, 0.0);
    RayTracerChallenge::Ray ray(origin, direction);
    CHECK(ray.position(0.0) == RayTracerChallenge::Tuple::point(2.0, 3.0, 4.0));
    CHECK(ray.position(1.0) == RayTracerChallenge::Tuple::point(3.0, 3.0, 4.0));
    CHECK(ray.position(-1.0) == RayTracerChallenge::Tuple::point(1.0, 3.0, 4.0));
    CHECK(ray.position(2.5) == RayTracerChallenge::Tuple::point(4.5, 3.0, 4.0));
  }
  SUBCASE("An intersection encapsulates t and object") {
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Intersection intersection(3.5, sphere);
    CHECK(intersection.t == 3.5);
    CHECK(*intersection.object == *sphere);
  }
  SUBCASE("Aggregating intersections") {
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Intersection intersection1(1.0, sphere);
    RayTracerChallenge::Intersection intersection2(2.0, sphere);
    std::vector<RayTracerChallenge::Intersection> intersections{intersection1, intersection2};
    CHECK(intersections.size() == 2);
    CHECK(*intersections[0].object == *sphere);
    CHECK(*intersections[1].object == *sphere);
  }
  SUBCASE("A ray intersects a sphere at two points") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Intersections intersections = sphere->intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == 4.0);
    CHECK(intersections[1].t == 6.0);
    CHECK(*sphere == *intersections[0].object);
    CHECK(*sphere == *intersections[1].object);
  }
  SUBCASE("A ray intersects a sphere at a tangent") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 1.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Intersections intersections = sphere->intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == 5.0);
    CHECK(intersections[1].t == 5.0);
    CHECK(*sphere == *intersections[0].object);
    CHECK(*sphere == *intersections[1].object);
  }
  SUBCASE("A ray originates inside a sphere") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Intersections intersections = sphere->intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == -1.0);
    CHECK(intersections[1].t == 1.0);
    CHECK(*sphere == *intersections[0].object);
    CHECK(*sphere == *intersections[1].object);
  }
  SUBCASE("A sphere is behind a ray") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, 5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Intersections intersections = sphere->intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == -6.0);
    CHECK(intersections[1].t == -4.0);
    CHECK(*sphere == *intersections[0].object);
    CHECK(*sphere == *intersections[1].object);
  }
  SUBCASE("The hit, when all intersections have positive t") {
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Intersection intersection1(1.0, sphere);
    RayTracerChallenge::Intersection intersection2(1.0, sphere);
    RayTracerChallenge::Intersections xs({intersection1, intersection2});
    CHECK(xs.hit() == intersection1);
  }
  SUBCASE("The hit, when some intersections have negative t") {
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Intersection intersection1(-1.0, sphere);
    RayTracerChallenge::Intersection intersection2(1.0, sphere);
    RayTracerChallenge::Intersections xs({intersection1, intersection2});
    CHECK(xs.hit() == intersection2);
  }
  SUBCASE("The hit, when all intersections have negative t") {
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Intersection intersection1(-2.0, sphere);
    RayTracerChallenge::Intersection intersection2(-1.0, sphere);
    RayTracerChallenge::Intersections xs({intersection1, intersection2});
    CHECK(xs.hit().has_value() == false);
  }
  SUBCASE("The hit is always the lowest nonnegative intersection") {
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Intersection intersection1(5.0, sphere);
    RayTracerChallenge::Intersection intersection2(7.0, sphere);
    RayTracerChallenge::Intersection intersection3(-3.0, sphere);
    RayTracerChallenge::Intersection intersection4(2.0, sphere);
    RayTracerChallenge::Intersections xs(
        {intersection1, intersection2, intersection3, intersection4});
    CHECK(xs.hit() == intersection4);
  }
  SUBCASE("Translating a ray") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(1.0, 2.0, 3.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Matrix matrix = RayTracerChallenge::Matrix::translation(3.0, 4.0, 5.0);
    RayTracerChallenge::Ray ray2 = ray.transform(matrix);
    CHECK(ray2.origin == RayTracerChallenge::Tuple::point(4.0, 6.0, 8.0));
    CHECK(ray2.direction == RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0));
  }
  SUBCASE("Scaling a ray") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(1.0, 2.0, 3.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Matrix matrix = RayTracerChallenge::Matrix::scaling(2.0, 3.0, 4.0);
    RayTracerChallenge::Ray ray2 = ray.transform(matrix);
    CHECK(ray2.origin == RayTracerChallenge::Tuple::point(2.0, 6.0, 12.0));
    CHECK(ray2.direction == RayTracerChallenge::Tuple::vector(0.0, 3.0, 0.0));
  }
  SUBCASE("A sphere's default transformation") {
    auto sphere = RayTracerChallenge::Sphere::create();
    CHECK(sphere->transform == RayTracerChallenge::Matrix::identity(4));
  }
  SUBCASE("Changing a sphere's transformation") {
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Matrix matrix = RayTracerChallenge::Matrix::translation(2.0, 3.0, 4.0);
    sphere->transform = matrix;
    CHECK(sphere->transform == matrix);
  }
  SUBCASE("Intersecting a scaled sphere with a ray") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    auto sphere = RayTracerChallenge::Sphere::create();
    sphere->transform = RayTracerChallenge::Matrix::scaling(2.0, 2.0, 2.0);
    RayTracerChallenge::Intersections xs = sphere->intersect(ray);
    CHECK(xs.size() == 2);
    CHECK(xs[0].t == 3.0);
    CHECK(xs[1].t == 7.0);
  }
  SUBCASE("Intersecting a translated sphere with a ray") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    auto sphere = RayTracerChallenge::Sphere::create();
    sphere->transform = RayTracerChallenge::Matrix::translation(5.0, 0.0, 0.0);
    RayTracerChallenge::Intersections xs = sphere->intersect(ray);
    CHECK(xs.size() == 0);
  }
  SUBCASE("Precomputing the state of an intersection") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Intersection intersection(4.0, sphere);
    RayTracerChallenge::Computations computations = intersection.prepareComputations(ray);
    CHECK(computations.t == intersection.t);
    CHECK(computations.object->is(*intersection.object));
    CHECK(computations.point == RayTracerChallenge::Tuple::point(0.0, 0.0, -1.0));
    CHECK(computations.eyeVector == RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0));
    CHECK(computations.normalVector == RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0));
  }
  SUBCASE("The hit, when an intersection occurs on the outside") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Intersection intersection(4.0, sphere);
    RayTracerChallenge::Computations computations = intersection.prepareComputations(ray);
    CHECK(computations.inside == false);
  }
  SUBCASE("The hit, when an intersection occurs on the inside") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    auto sphere = RayTracerChallenge::Sphere::create();
    RayTracerChallenge::Intersection intersection(1.0, sphere);
    RayTracerChallenge::Computations computations = intersection.prepareComputations(ray);
    CHECK(computations.point == RayTracerChallenge::Tuple::point(0.0, 0.0, 1.0));
    CHECK(computations.eyeVector == RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0));
    CHECK(computations.normalVector == RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0));
    CHECK(computations.inside == true);
  }
  SUBCASE("The hit should offset the point") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    auto sphere = RayTracerChallenge::Sphere::create();
    sphere->transform = RayTracerChallenge::Matrix::translation(0.0, 0.0, 1.0);
    RayTracerChallenge::Intersection intersection(5.0, sphere);
    RayTracerChallenge::Computations computations = intersection.prepareComputations(ray);
    CHECK(computations.overPoint.z < 0.0001 / 2.0);
    CHECK(computations.point.z > computations.overPoint.z);
  }
}

TEST_CASE("Normals") {
  using namespace raytracerchallenge;
  SUBCASE("The normal on a sphere at a point on the x axis") {
    auto sphere = RayTracerChallenge::Sphere::create();
    auto n = sphere->normalAt(RayTracerChallenge::Tuple::point(1.0, 0.0, 0.0));
    CHECK(n == RayTracerChallenge::Tuple::vector(1.0, 0.0, 0.0));
  }
  SUBCASE("The normal on a sphere at a point on the y axis") {
    auto sphere = RayTracerChallenge::Sphere::create();
    auto n = sphere->normalAt(RayTracerChallenge::Tuple::point(0.0, 1.0, 0.0));
    CHECK(n == RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0));
  }
  SUBCASE("The normal on a sphere at a point on the z axis") {
    auto sphere = RayTracerChallenge::Sphere::create();
    auto n = sphere->normalAt(RayTracerChallenge::Tuple::point(0.0, 0.0, 1.0));
    CHECK(n == RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0));
  }
  SUBCASE("The normal on a sphere at a nonaxial point") {
    auto sphere = RayTracerChallenge::Sphere::create();
    auto n = sphere->normalAt(
        RayTracerChallenge::Tuple::point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
    CHECK(n
          == RayTracerChallenge::Tuple::vector(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
  }
  SUBCASE("The normal is a normalized vector") {
    auto sphere = RayTracerChallenge::Sphere::create();
    auto n = sphere->normalAt(
        RayTracerChallenge::Tuple::point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
    CHECK(n == n.normalize());
  }
  SUBCASE("Computing the normal on a translated sphere") {
    auto sphere = RayTracerChallenge::Sphere::create();
    sphere->transform = RayTracerChallenge::Matrix::translation(0.0, 1.0, 0.0);
    auto n = sphere->normalAt(RayTracerChallenge::Tuple::point(0.0, 1.70711, -0.70711));
    CHECK(n == RayTracerChallenge::Tuple::vector(0.0, 0.70711, -0.70711));
  }
  SUBCASE("Computing the normal on a transformed sphere") {
    auto sphere = RayTracerChallenge::Sphere::create();
    sphere->transform = (RayTracerChallenge::Matrix::scaling(1.0, 0.5, 1.0)
                         * RayTracerChallenge::Matrix::rotationZ(M_PI / 5.0));
    auto n = sphere->normalAt(
        RayTracerChallenge::Tuple::point(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0));
    CHECK(n == RayTracerChallenge::Tuple::vector(0.0, 0.97014, -0.24254));
  }
}
TEST_CASE("Planes") {
  using namespace raytracerchallenge;
  SUBCASE("The normal of a plane is constant everywhere") {
    auto plane = RayTracerChallenge::Plane::create();
    auto n1 = plane->localNormalAt({0.0, 0.0, 0.0, 0.0});
    auto n2 = plane->localNormalAt({10.0, 0.0, -10.0, 0.0});
    auto n3 = plane->localNormalAt({-5.0, 0.0, 150.0, 0.0});
    CHECK(n1 == RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0));
    CHECK(n2 == RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0));
    CHECK(n3 == RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0));
  }
  SUBCASE("Intersect with a ray parallel to the plane") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 10.0, 1.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    auto plane = RayTracerChallenge::Plane::create();
    auto xs = plane->localIntersect(ray);
    CHECK(xs.size() == 0);
  }
  SUBCASE("Intersect with a coplanar ray") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    auto plane = RayTracerChallenge::Plane::create();
    auto xs = plane->localIntersect(ray);
    CHECK(xs.size() == 0);
  }
  SUBCASE("A ray intersecting a plane from above") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 1.0, 0.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, -1.0, 0.0);
    RayTracerChallenge::Ray ray(origin, direction);
    auto plane = RayTracerChallenge::Plane::create();
    auto xs = plane->localIntersect(ray);
    CHECK(xs.size() == 1);
    CHECK(xs[0].t == 1);
    CHECK(xs[0].object == plane);
  }
  SUBCASE("A ray intersecting a plane from below") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, -1.0, 0.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
    RayTracerChallenge::Ray ray(origin, direction);
    auto plane = RayTracerChallenge::Plane::create();
    auto xs = plane->localIntersect(ray);
    CHECK(xs.size() == 1);
    CHECK(xs[0].t == 1);
    CHECK(xs[0].object == plane);
  }
}

TEST_CASE("Reflecting vectors") {
  using namespace raytracerchallenge;
  SUBCASE("Reflecting a vector approaching at 45 degrees") {
    auto v = RayTracerChallenge::Tuple::vector(1.0, -1.0, 0.0);
    auto n = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
    auto r = v.reflect(n);
    CHECK(r == RayTracerChallenge::Tuple::vector(1.0, 1.0, 0.0));
  }
  SUBCASE("Reflecting a vector off a slanted surface") {
    auto v = RayTracerChallenge::Tuple::vector(0.0, -1.0, 0.0);
    auto n = RayTracerChallenge::Tuple::vector(sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0);
    auto r = v.reflect(n);
    CHECK(r == RayTracerChallenge::Tuple::vector(1.0, 0.0, 0.0));
  }
}
TEST_CASE("Point Lights") {
  using namespace raytracerchallenge;
  SUBCASE("A point light has a position and intensity") {
    auto intensity = RayTracerChallenge::Color(1.0, 1.0, 1.0);
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto light = RayTracerChallenge::PointLight(position, intensity);
    CHECK(light.intensity == intensity);
    CHECK(light.position == position);
  }
}
TEST_CASE("Lighting") {
  using namespace raytracerchallenge;
  SUBCASE("Lighting with the eye between the light and the surface") {
    auto s = RayTracerChallenge::Sphere::create();
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.0, -10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == RayTracerChallenge::Color(1.9, 1.9, 1.9));
  }
  SUBCASE("Lighting with the eye between the light and the surface, eye offset 45 degrees") {
    auto s = RayTracerChallenge::Sphere::create();
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.0, -10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == RayTracerChallenge::Color(1.0, 1.0, 1.0));
  }
  SUBCASE("Lighting with the eye opposite surface, light offset 45 degrees") {
    auto s = RayTracerChallenge::Sphere::create();
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 10.0, -10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == RayTracerChallenge::Color(0.7364, 0.7364, 0.7364));
  }
  SUBCASE("Lighting with eye in the path of the reflection vector") {
    auto s = RayTracerChallenge::Sphere::create();
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, -sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 10.0, -10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == RayTracerChallenge::Color(1.63638, 1.63638, 1.63638));
  }
  SUBCASE("Lighting with the light behind the surface") {
    auto s = RayTracerChallenge::Sphere::create();
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.0, 10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == RayTracerChallenge::Color(0.1, 0.1, 0.1));
  }
  SUBCASE("Lighting with the surface in shadow") {
    auto s = RayTracerChallenge::Sphere::create();
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.0, -10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, true);
    CHECK(result == RayTracerChallenge::Color(0.1, 0.1, 0.1));
  }
  SUBCASE("Lighting with a pattern applied") {
    auto s = RayTracerChallenge::Sphere::create();
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    s->material.pattern = &pattern;
    s->material.ambient = 1.0;
    s->material.diffuse = 0.0;
    s->material.specular = 0.0;
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.0, -10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto c1 = RayTracerChallenge::lighting(s, light, {0.9, 0.0, 0.0, 1.0}, eyeVector, normalVector,
                                           false);
    auto c2 = RayTracerChallenge::lighting(s, light, {1.1, 0.0, 0.0, 1.0}, eyeVector, normalVector,
                                           false);
    CHECK(c1 == RayTracerChallenge::Color::WHITE);
    CHECK(c2 == RayTracerChallenge::Color::BLACK);
  }
}
TEST_CASE("World") {
  using namespace raytracerchallenge;
  SUBCASE("Creating a world") {
    RayTracerChallenge::World world;
    CHECK(world.isEmpty());
    CHECK(world.light.has_value() == false);
  }
  SUBCASE("The default world") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto sphere1 = RayTracerChallenge::Sphere::create();
    sphere1->material.color = RayTracerChallenge::Color(0.8, 1.0, 0.6);
    sphere1->material.diffuse = 0.7;
    sphere1->material.specular = 0.2;
    auto sphere2 = RayTracerChallenge::Sphere::create();
    sphere2->transform = RayTracerChallenge::Matrix::scaling(0.5, 0.5, 0.5);
    CHECK(world.light->position == RayTracerChallenge::Tuple::point(-10.0, 10.0, -10.0));
    CHECK(world.light->intensity == RayTracerChallenge::Color(1.0, 1.0, 1.0));
    CHECK(*sphere1 == *world.objects[0]);
    CHECK(*sphere2 == *world.objects[1]);
  }
  SUBCASE("Intersecting a ray with the world") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto ray = RayTracerChallenge::Ray(RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0),
                                       RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0));
    RayTracerChallenge::Intersections xs = world.intersect(ray);
    CHECK(xs.size() == 4);
    CHECK(xs[0].t == 4.0);
    CHECK(xs[1].t == 4.5);
    CHECK(xs[2].t == 5.5);
    CHECK(xs[3].t == 6.0);
  }
  SUBCASE("Shading an intersection") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto ray = RayTracerChallenge::Ray(RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0),
                                       RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0));
    auto shape = world.objects[0];
    auto intersection = RayTracerChallenge::Intersection(4.0, shape);
    auto computations = intersection.prepareComputations(ray);
    auto color = world.shadeHit(computations, 4);
    CHECK(color == RayTracerChallenge::Color(0.38066, 0.47583, 0.2855));
  }
  SUBCASE("Shading an intersection from the inside") {
    auto world = RayTracerChallenge::World::defaultWorld();
    world.light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.25, 0.0),
                                                 RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto ray = RayTracerChallenge::Ray(RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0),
                                       RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0));
    auto shape = world.objects[1];
    auto intersection = RayTracerChallenge::Intersection(.5, shape);
    auto computations = intersection.prepareComputations(ray);
    auto color = world.shadeHit(computations, 4);
    CHECK(color == RayTracerChallenge::Color(0.90498, 0.90498, 0.90498));
  }
  SUBCASE("Shading an intersection in shadow") {
    auto world = RayTracerChallenge::World();
    world.light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.0, 10.0),
                                                 RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto sphere = RayTracerChallenge::Sphere::create();
    world.add(sphere);
    auto sphere2 = RayTracerChallenge::Sphere::create();
    sphere2->transform = RayTracerChallenge::Matrix::translation(0.0, 0.0, 10.0);
    world.add(sphere2);
    auto ray = RayTracerChallenge::Ray(RayTracerChallenge::Tuple::point(0.0, 0.0, 5.0),
                                       RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0));
    auto intersection = RayTracerChallenge::Intersection(4.0, sphere2);
    auto computations = intersection.prepareComputations(ray);
    auto color = world.shadeHit(computations, 4);
    CHECK(color == RayTracerChallenge::Color(0.1, 0.1, 0.1));
  }
  SUBCASE("The color when a ray misses") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto ray = RayTracerChallenge::Ray(RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0),
                                       RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0));
    auto color = world.colorAt(ray, 4);
    CHECK(color == RayTracerChallenge::Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The color when a ray hits") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto ray = RayTracerChallenge::Ray(RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0),
                                       RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0));
    auto color = world.colorAt(ray, 4);
    CHECK(color == RayTracerChallenge::Color(0.38066, 0.47583, 0.2855));
  }
  SUBCASE("The color with an intersection behind the ray") {
    auto world = RayTracerChallenge::World::defaultWorld();
    world.objects[0]->material.ambient = 1.0;
    world.objects[1]->material.ambient = 1.0;
    auto ray = RayTracerChallenge::Ray(RayTracerChallenge::Tuple::point(0.0, 0.0, 0.75),
                                       RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0));
    auto color = world.colorAt(ray, 4);
    CHECK(color == world.objects[1]->material.color);
  }
  SUBCASE("There is no shadow when nothing is collinear with the point and light") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto point = RayTracerChallenge::Tuple::point(0.0, 10.0, 0.0);
    CHECK(world.isShadowed(point) == false);
  }
  SUBCASE("The shadow when an object is between the point and the light") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto point = RayTracerChallenge::Tuple::point(10.0, -10.0, 10.0);
    CHECK(world.isShadowed(point) == true);
  }
  SUBCASE("There is no shadow when an object is behind the light") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto point = RayTracerChallenge::Tuple::point(-20.0, 20.0, -20.0);
    CHECK(world.isShadowed(point) == false);
  }
  SUBCASE("There is no shadow when the object is behind the point") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto point = RayTracerChallenge::Tuple::point(-2.0, 0.0, -2.0);
    CHECK(world.isShadowed(point) == false);
  }
}
TEST_CASE("Camera") {
  using namespace raytracerchallenge;
  SUBCASE("Constructing a camera") {
    auto camera = RayTracerChallenge::Camera(160, 120, M_PI / 2.0);
    CHECK(camera.hSize == 160);
    CHECK(camera.vSize == 120);
    CHECK(camera.fieldOfView == M_PI / 2.0);
    CHECK(camera.transform == RayTracerChallenge::Matrix::identity(4));
  }
  SUBCASE("The pixel size for a horizontal canvas") {
    auto camera = RayTracerChallenge::Camera(200, 125, M_PI / 2.0);
    CHECK(abs(camera.pixelSize - 0.01) < 0.0001);
  }
  SUBCASE("The pixel size for a vertical canvas") {
    auto camera = RayTracerChallenge::Camera(125, 200, M_PI / 2.0);
    CHECK(abs(camera.pixelSize - 0.01) < 0.0001);
  }
  SUBCASE("Constructing a ray through the center of the canvas") {
    auto camera = RayTracerChallenge::Camera(201, 101, M_PI / 2.0);
    auto ray = camera.rayForPixel(100, 50);
    CHECK(ray.origin == RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0));
    CHECK(ray.direction == RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0));
  }
  SUBCASE("Constructing a ray through a corner of the canvas") {
    auto camera = RayTracerChallenge::Camera(201, 101, M_PI / 2.0);
    auto ray = camera.rayForPixel(0, 0);
    CHECK(ray.origin == RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0));
    CHECK(ray.direction == RayTracerChallenge::Tuple::vector(0.66519, 0.33259, -0.66851));
  }
  SUBCASE("Constructing a ray when the camera is transformed") {
    auto camera = RayTracerChallenge::Camera(201, 101, M_PI / 2.0);
    camera.transform
        = RayTracerChallenge::Matrix::identity(4).translated(0.0, -2.0, 5.0).rotatedY(M_PI / 4.0);
    auto ray = camera.rayForPixel(100, 50);
    CHECK(ray.origin == RayTracerChallenge::Tuple::point(0.0, 2.0, -5.0));
    CHECK(ray.direction
          == RayTracerChallenge::Tuple::vector(sqrt(2.0) / 2.0, 0.0, -sqrt(2.0) / 2.0));
  }
  SUBCASE("Rendering a world with a camera") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto camera = RayTracerChallenge::Camera(11, 11, M_PI / 2.0);
    auto from = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    auto to = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto up = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
    camera.transform = RayTracerChallenge::Matrix::view(from, to, up);
    auto image = camera.render(world);
    CHECK(image.pixelAt(5, 5) == RayTracerChallenge::Color(0.38066, 0.47583, 0.2855));
  }
}
TEST_CASE("Patterns") {
  using namespace raytracerchallenge;
  SUBCASE("A stripe pattern is constant in y") {
    auto shape = RayTracerChallenge::Sphere::create();
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 1.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 2.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("A stripe pattern is constant in z") {
    auto shape = RayTracerChallenge::Sphere::create();
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 1.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 2.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("A stripe pattern alternates in x") {
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    auto shape = RayTracerChallenge::Sphere::create();
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.9, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {1.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {-0.1, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {-1.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {-1.1, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("Stripes with an object transformation") {
    auto object = RayTracerChallenge::Sphere::create();
    object->transform = RayTracerChallenge::Matrix::scaling(2.0, 2.0, 2.0);
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(object, {1.5, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("Stripes with an pattern transformation") {
    auto object = RayTracerChallenge::Sphere::create();
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    pattern.transform = RayTracerChallenge::Matrix::scaling(2.0, 2.0, 2.0);
    CHECK(pattern.colorAt(object, {1.5, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("Stripes with both an object and pattern transformation") {
    auto object = RayTracerChallenge::Sphere::create();
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    object->transform = RayTracerChallenge::Matrix::scaling(2.0, 2.0, 2.0);
    pattern.transform = RayTracerChallenge::Matrix::translation(0.5, 0.0, 0.0);
    CHECK(pattern.colorAt(object, {2.5, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("A gradient linearly interpolates between colors") {
    auto shape = RayTracerChallenge::Sphere::create();
    auto pattern = RayTracerChallenge::GradientPattern(RayTracerChallenge::Color::WHITE,
                                                       RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.25, 0.0, 0.0, 1.0})
          == RayTracerChallenge::Color(0.75, 0.75, 0.75));
    CHECK(pattern.colorAt(shape, {0.5, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color(0.5, 0.5, 0.5));
    CHECK(pattern.colorAt(shape, {0.75, 0.0, 0.0, 1.0})
          == RayTracerChallenge::Color(0.25, 0.25, 0.25));
  }
  SUBCASE("A ring should extend in both x and z") {
    auto shape = RayTracerChallenge::Sphere::create();
    auto pattern = RayTracerChallenge::RingPattern(RayTracerChallenge::Color::WHITE,
                                                   RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {1.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 1.0, 1.0}) == RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.708, 0.0, 0.708, 1.0}) == RayTracerChallenge::Color::BLACK);
  }
  SUBCASE("Checkers should repeat in x") {
    auto shape = RayTracerChallenge::Sphere::create();
    auto pattern = RayTracerChallenge::CheckersPattern(RayTracerChallenge::Color::WHITE,
                                                       RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.99, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {1.01, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::BLACK);
  }
  SUBCASE("Checkers should repeat in y") {
    auto shape = RayTracerChallenge::Sphere::create();
    auto pattern = RayTracerChallenge::CheckersPattern(RayTracerChallenge::Color::WHITE,
                                                       RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.99, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 1.01, 0.0, 1.0}) == RayTracerChallenge::Color::BLACK);
  }
  SUBCASE("Checkers should repeat in z") {
    auto shape = RayTracerChallenge::Sphere::create();
    auto pattern = RayTracerChallenge::CheckersPattern(RayTracerChallenge::Color::WHITE,
                                                       RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.99, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 1.01, 1.0}) == RayTracerChallenge::Color::BLACK);
  }
}
TEST_CASE("Reflection") {
  using namespace raytracerchallenge;
  SUBCASE("Refectivity for the default material") {
    RayTracerChallenge::Material material;
    CHECK(material.reflective == 0.0);
  }
  SUBCASE("Precomputing the reflection vector") {
    auto shape = RayTracerChallenge::Plane::create();
    auto r = RayTracerChallenge::Ray({0.0, 1.0, -1.0, 1.0},
                                     {0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0});
    auto i = RayTracerChallenge::Intersection(sqrt(2.0), shape);
    auto comps = i.prepareComputations(r);
    CHECK(comps.reflectionVector
          == RayTracerChallenge::Tuple::vector(0.0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));
  }
  SUBCASE("The reflected color for a nonreflective material") {
    auto w = RayTracerChallenge::World::defaultWorld();
    auto r = RayTracerChallenge::Ray({0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto shape = w.objects[1];
    shape->material.ambient = 1.0;
    auto i = RayTracerChallenge::Intersection(1.0, shape);
    auto comps = i.prepareComputations(r);
    auto color = w.reflectedColorAt(comps, 4);
    CHECK(color == RayTracerChallenge::Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The reflected color for a reflective material") {
    auto w = RayTracerChallenge::World::defaultWorld();
    auto shape = RayTracerChallenge::Plane::create();
    shape->material.reflective = 0.5;
    shape->transform = shape->transform.translated(0.0, -1.0, 0.0);
    w.add(shape);
    auto r = RayTracerChallenge::Ray({0.0, 0.0, -3.0, 1.0},
                                     {0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0});
    auto i = RayTracerChallenge::Intersection(sqrt(2.0), shape);
    auto comps = i.prepareComputations(r);
    auto color = w.reflectedColorAt(comps, 4);
    CHECK(color == RayTracerChallenge::Color(0.19032, 0.2379, 0.14274));
  }
  SUBCASE("Shading a hit for a reflective material") {
    auto w = RayTracerChallenge::World::defaultWorld();
    auto shape = RayTracerChallenge::Plane::create();
    shape->material.reflective = 0.5;
    shape->transform = shape->transform.translated(0.0, -1.0, 0.0);
    w.add(shape);
    auto r = RayTracerChallenge::Ray({0.0, 0.0, -3.0, 1.0},
                                     {0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0});
    auto i = RayTracerChallenge::Intersection(sqrt(2.0), shape);
    auto comps = i.prepareComputations(r);
    auto color = w.shadeHit(comps, 4);
    CHECK(color == RayTracerChallenge::Color(0.87677, 0.92436, 0.82918));
  }
  SUBCASE("Coloring mutually reflective surfaces") {
    auto w = RayTracerChallenge::World::defaultWorld();
    w.light = RayTracerChallenge::PointLight({0.0, 0.0, 0.0, 1.0}, {1.0, 1.0, 1.0});
    auto lower = RayTracerChallenge::Plane::create();
    lower->material.reflective = 1.0;
    lower->transform = lower->transform.translated(0.0, -1.0, 0.0);
    w.add(lower);
    auto upper = RayTracerChallenge::Plane::create();
    upper->material.reflective = 1.0;
    upper->transform = upper->transform.translated(0.0, 1.0, 0.0);
    w.add(upper);
    auto r = RayTracerChallenge::Ray({0.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 0.0});
    w.colorAt(r, 4);
  }
  SUBCASE("The reflected color for a reflective material at maximum recursion depth") {
    auto w = RayTracerChallenge::World::defaultWorld();
    auto shape = RayTracerChallenge::Plane::create();
    shape->material.reflective = 0.5;
    shape->transform = shape->transform.translated(0.0, -1.0, 0.0);
    w.add(shape);
    auto r = RayTracerChallenge::Ray({0.0, 0.0, -3.0, 1.0},
                                     {0.0, -sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0});
    auto i = RayTracerChallenge::Intersection(sqrt(2.0), shape);
    auto comps = i.prepareComputations(r);
    auto color = w.reflectedColorAt(comps, 0);
    CHECK(color == RayTracerChallenge::Color(0.0, 0.0, 0.0));
  }
}
TEST_CASE("Transparency and refraction") {
  using namespace raytracerchallenge;
  SUBCASE("Transparency and refractive index for the default material") {
    RayTracerChallenge::Material material;
    CHECK(material.transparency == 0.0);
    CHECK(material.refractiveIndex == 1.0);
  }
  SUBCASE("A helper for producting a sphere with a glassy material") {
    auto glassSphere = RayTracerChallenge::GlassSphere::create();
    CHECK(glassSphere->material.transparency == 1.0);
    CHECK(glassSphere->material.refractiveIndex == 1.5);
  }
  SUBCASE("Finding n1 and n2 at various intersections") {
    auto a = RayTracerChallenge::GlassSphere::create();
    a->transform = a->transform.scaled(2.0, 2.0, 2.0);
    auto b = RayTracerChallenge::GlassSphere::create();
    b->transform = b->transform.translated(0.0, 0.0, -0.25);
    b->material.refractiveIndex = 2.0;
    auto c = RayTracerChallenge::GlassSphere::create();
    c->transform = c->transform.translated(0.0, 0.0, 0.25);
    c->material.refractiveIndex = 2.5;
    auto r = RayTracerChallenge::Ray({0.0, 0.0, -4.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = RayTracerChallenge::Intersections({
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
    auto r = RayTracerChallenge::Ray({0.0, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto shape = RayTracerChallenge::GlassSphere::create();
    shape->transform = shape->transform.translated(0.0, 0.0, 1.0);
    auto intersection = RayTracerChallenge::Intersection(5.0, shape);
    auto comps = intersection.prepareComputations(r);
    CHECK(comps.underPoint.z > 0.0001 / 2.0);
    CHECK(comps.point.z < comps.underPoint.z);
  }
  SUBCASE("The refracted color with an opaque surface") {
    auto w = RayTracerChallenge::World::defaultWorld();
    auto shape = w.objects[0];
    auto r = RayTracerChallenge::Ray({0.0, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = RayTracerChallenge::Intersections({{4.0, shape}, {6.0, shape}});
    auto comps = xs[0].prepareComputations(r, xs);
    CHECK(w.refractedColorAt(comps, 5) == RayTracerChallenge::Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The refracted color at the maximum recursive depth") {
    auto w = RayTracerChallenge::World::defaultWorld();
    auto shape = w.objects[0];
    shape->material.refractiveIndex = 1.5;
    shape->material.transparency = 1.0;
    auto r = RayTracerChallenge::Ray({0.0, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = RayTracerChallenge::Intersections({{4.0, shape}, {6.0, shape}});
    auto comps = xs[0].prepareComputations(r, xs);
    CHECK(w.refractedColorAt(comps, 0) == RayTracerChallenge::Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The refracted color under total internal reflection") {
    auto w = RayTracerChallenge::World::defaultWorld();
    auto shape = w.objects[0];
    shape->material.refractiveIndex = 1.5;
    shape->material.transparency = 1.0;
    auto r = RayTracerChallenge::Ray({0.0, 0.0, sqrt(2.0 / 2.0), 1.0}, {0.0, 1.0, 0.0, 0.0});
    auto xs
        = RayTracerChallenge::Intersections({{-sqrt(2.0) / 2.0, shape}, {sqrt(2.0) / 2.0, shape}});
    auto comps = xs[1].prepareComputations(r, xs);
    CHECK(w.refractedColorAt(comps, 5) == RayTracerChallenge::Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The refracted color with a refracted ray") {
    class TestPattern : public RayTracerChallenge::Pattern {
    public:
      [[nodiscard]] RayTracerChallenge::Color colorAt(
          std::shared_ptr<RayTracerChallenge::Shape> shape,
          RayTracerChallenge::Tuple point) const override {
        (void)shape;
        return {point.x, point.y, point.z};
      }
    };
    auto w = RayTracerChallenge::World::defaultWorld();
    auto a = w.objects[0];
    a->material.ambient = 1.0;
    auto pattern = TestPattern();
    a->material.pattern = &pattern;
    auto b = w.objects[1];
    b->material.transparency = 1.0;
    b->material.refractiveIndex = 1.5;
    auto r = RayTracerChallenge::Ray({0.0, 0.0, 0.1, 1.0}, {0.0, 1.0, 0.0, 0.0});
    auto xs = RayTracerChallenge::Intersections({
        {-0.9899, a},
        {-0.4899, b},
        {0.4899, b},
        {0.9899, a},
    });
    auto comps = xs[2].prepareComputations(r, xs);
    CHECK(w.refractedColorAt(comps, 5) == RayTracerChallenge::Color(0.0, 0.99888, 0.04725));
  }
  SUBCASE("Shading a hit on a transparent material") {
    auto w = RayTracerChallenge::World::defaultWorld();
    auto floor = RayTracerChallenge::Plane::create();
    floor->transform = RayTracerChallenge::Matrix::translation(0.0, -1.0, 0.0);
    floor->material.refractiveIndex = 1.5;
    floor->material.transparency = 0.5;
    w.add(floor);
    auto ball = RayTracerChallenge::Sphere::create();
    ball->material.color = RayTracerChallenge::Color(1.0, 0.0, 0.0);
    ball->material.ambient = 0.5;
    ball->transform = RayTracerChallenge::Matrix::translation(0.0, -3.5, -0.5);
    w.add(ball);
    auto r
        = RayTracerChallenge::Ray({0.0, 0.0, -3.0, 1.0}, {0.0, -sqrt(2.0) / 2, sqrt(2.0) / 2, 0.0});
    auto xs = RayTracerChallenge::Intersections({{sqrt(2.0), floor}});
    auto comps = xs[0].prepareComputations(r, xs);
    auto color = w.shadeHit(comps, 5);
    CHECK(color == RayTracerChallenge::Color(0.93642, 0.68642, 0.68642));
  }
  SUBCASE("The Schlick approximation under total internal reflection") {
    auto shape = RayTracerChallenge::GlassSphere::create();
    auto r = RayTracerChallenge::Ray({0.0, 0.0, sqrt(2.0) / 2.0, 1.0}, {0.0, 1.0, 0.0, 0.0});
    auto xs
        = RayTracerChallenge::Intersections({{-sqrt(2.0) / 2.0, shape}, {sqrt(2.0) / 2.0, shape}});
    auto comps = xs[1].prepareComputations(r, xs);
    auto reflectance = RayTracerChallenge::Computations::schlick(comps);
    CHECK(reflectance == 1.0);
  }
  SUBCASE("The Schlick approximation with a perpendicular viewing angle") {
    auto shape = RayTracerChallenge::GlassSphere::create();
    auto r = RayTracerChallenge::Ray({0.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 0.0});
    auto xs = RayTracerChallenge::Intersections({{-1.0, shape}, {1.0, shape}});
    auto comps = xs[1].prepareComputations(r, xs);
    auto reflectance = RayTracerChallenge::Computations::schlick(comps);
    CHECK(abs(reflectance - 0.04) < 0.0001);
  }
  SUBCASE("The Schlick approximation with a small angle and n2 > n1") {
    auto shape = RayTracerChallenge::GlassSphere::create();
    auto r = RayTracerChallenge::Ray({0.0, 0.99, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = RayTracerChallenge::Intersections({{1.8589, shape}});
    auto comps = xs[0].prepareComputations(r, xs);
    auto reflectance = RayTracerChallenge::Computations::schlick(comps);
    CHECK(abs(reflectance - 0.48873) < 0.0001);
  }
  SUBCASE("Shading a hit on a reflective, transparent material") {
    auto w = RayTracerChallenge::World::defaultWorld();
    auto floor = RayTracerChallenge::Plane::create();
    floor->transform = RayTracerChallenge::Matrix::translation(0.0, -1.0, 0.0);
    floor->material.refractiveIndex = 1.5;
    floor->material.reflective = 0.5;
    floor->material.transparency = 0.5;
    w.add(floor);
    auto ball = RayTracerChallenge::Sphere::create();
    ball->material.color = RayTracerChallenge::Color(1.0, 0.0, 0.0);
    ball->material.ambient = 0.5;
    ball->transform = RayTracerChallenge::Matrix::translation(0.0, -3.5, -0.5);
    w.add(ball);
    auto r
        = RayTracerChallenge::Ray({0.0, 0.0, -3.0, 1.0}, {0.0, -sqrt(2.0) / 2, sqrt(2.0) / 2, 0.0});
    auto xs = RayTracerChallenge::Intersections({{sqrt(2.0), floor}});
    auto comps = xs[0].prepareComputations(r, xs);
    auto color = w.shadeHit(comps, 5);
    CHECK(color == RayTracerChallenge::Color(0.93391, 0.69643, 0.69243));
  }
}
static RayTracerChallenge::Intersections getIntersectionsForCube(
    const std::shared_ptr<RayTracerChallenge::Shape>& cube, RayTracerChallenge::Tuple origin,
    RayTracerChallenge::Tuple direction) {
  auto r = RayTracerChallenge::Ray(origin, direction);
  return cube->localIntersect(r);
}
TEST_CASE("Cubes") {
  using namespace raytracerchallenge;
  SUBCASE("A ray intersects a cube") {
    auto cube = RayTracerChallenge::Cube::create();
    auto res1 = getIntersectionsForCube(cube, {5.0, 0.5, 0.0, 1.0}, {-1.0, 0.0, 0.0, 0.0});
    CHECK(res1[0].t == 4.0);
    CHECK(res1[1].t == 6.0);
    auto res2 = getIntersectionsForCube(cube, {-5.0, 0.5, 0.0, 1.0}, {1.0, 0.0, 0.0, 0.0});
    CHECK(res2[0].t == 4.0);
    CHECK(res2[1].t == 6.0);
    auto res3 = getIntersectionsForCube(cube, {0.5, 5.0, 0.0, 1.0}, {0.0, -1.0, 0.0, 0.0});
    CHECK(res3[0].t == 4.0);
    CHECK(res3[1].t == 6.0);
    auto res4 = getIntersectionsForCube(cube, {0.5, -5.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 0.0});
    CHECK(res4[0].t == 4.0);
    CHECK(res4[1].t == 6.0);
    auto res5 = getIntersectionsForCube(cube, {0.5, 0.0, 5.0, 1.0}, {0.0, 0.0, -1.0, 0.0});
    CHECK(res5[0].t == 4.0);
    CHECK(res5[1].t == 6.0);
    auto res6 = getIntersectionsForCube(cube, {0.5, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    CHECK(res6[0].t == 4.0);
    CHECK(res6[1].t == 6.0);
    auto res7 = getIntersectionsForCube(cube, {0.0, 0.5, 0.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    CHECK(res7[0].t == -1.0);
    CHECK(res7[1].t == 1.0);
  }
  SUBCASE("A ray misses a cube") {
    auto cube = RayTracerChallenge::Cube::create();
    auto res1 = getIntersectionsForCube(cube, {-2.0, 0.0, 0.0, 1.0}, {0.2673, 0.5345, 0.8018, 0.0});
    CHECK(res1.size() == 0);
    auto res2 = getIntersectionsForCube(cube, {0.0, -2.0, 0.0, 1.0}, {0.8018, 0.2673, 0.5345, 0.0});
    CHECK(res2.size() == 0);
    auto res3 = getIntersectionsForCube(cube, {0.0, 0.0, -2.0, 1.0}, {0.5345, 0.8018, 0.2673, 0.0});
    CHECK(res3.size() == 0);
    auto res4 = getIntersectionsForCube(cube, {2.0, 0.0, 2.0, 1.0}, {0.0, 0.0, -1.0, 0.0});
    CHECK(res4.size() == 0);
    auto res5 = getIntersectionsForCube(cube, {0.0, 2.0, 2.0, 1.0}, {0.0, -1.0, 0.0, 0.0});
    CHECK(res5.size() == 0);
    auto res6 = getIntersectionsForCube(cube, {2.0, 2.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 0.0});
    CHECK(res6.size() == 0);
  }
  SUBCASE("The normal on the surface of a cube") {
    auto cube = RayTracerChallenge::Cube::create();
    CHECK(cube->localNormalAt({1.0, 0.5, -0.8, 0.0})
          == RayTracerChallenge::Tuple({1.0, 0.0, 0.0, 0.0}));
    CHECK(cube->localNormalAt({-1.0, -0.2, 0.9, 0.0})
          == RayTracerChallenge::Tuple({-1.0, 0.0, 0.0, 0.0}));
    CHECK(cube->localNormalAt({-0.4, 1.0, -0.1, 0.0})
          == RayTracerChallenge::Tuple({0.0, 1.0, 0.0, 0.0}));
    CHECK(cube->localNormalAt({0.3, -1.0, -0.7, 0.0})
          == RayTracerChallenge::Tuple({0.0, -1.0, 0.0, 0.0}));
    CHECK(cube->localNormalAt({-0.6, 0.3, 1.0, 0.0})
          == RayTracerChallenge::Tuple({0.0, 0.0, 1.0, 0.0}));
    CHECK(cube->localNormalAt({0.4, 0.4, -1.0, 0.0})
          == RayTracerChallenge::Tuple({0.0, 0.0, -1.0, 0.0}));
    CHECK(cube->localNormalAt({1.0, 1.0, 1.0, 0.0})
          == RayTracerChallenge::Tuple({1.0, 0.0, 0.0, 0.0}));
    CHECK(cube->localNormalAt({-1.0, -1.0, -1.0, 0.0})
          == RayTracerChallenge::Tuple({-1.0, 0.0, 0.0, 0.0}));
  }
}
static RayTracerChallenge::Intersections getIntersectionsForCylinder(
    const std::shared_ptr<RayTracerChallenge::Shape>& cone, RayTracerChallenge::Tuple origin,
    RayTracerChallenge::Tuple direction) {
  auto r = RayTracerChallenge::Ray(origin, direction);
  return cone->localIntersect(r);
}
TEST_CASE("Cylinders") {
  using namespace raytracerchallenge;
  SUBCASE("A ray misses a cone") {
    auto cone = RayTracerChallenge::Cylinder::create();
    auto res1
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(1.0, 0.0, 0.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, 1.0, 0.0, 0.0).normalize());
    CHECK(res1.size() == 0);
    auto res2
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 0.0, 0.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, 1.0, 0.0, 0.0).normalize());
    CHECK(res2.size() == 0);
    auto res3
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 0.0, -5.0, 1.0),
                                      RayTracerChallenge::Tuple(1.0, 1.0, 1.0, 0.0).normalize());
    CHECK(res3.size() == 0);
  }
  SUBCASE("A ray strikes a cone") {
    auto cone = RayTracerChallenge::Cylinder::create();
    auto res1
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(1.0, 0.0, -5.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res1[0].t == 5.0);
    CHECK(res1[1].t == 5.0);
    auto res2
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 0.0, -5.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res2[0].t == 4.0);
    CHECK(res2[1].t == 6.0);
    auto res3
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.5, 0.0, -5.0, 1.0),
                                      RayTracerChallenge::Tuple(0.1, 1.0, 1.0, 0.0).normalize());
    CHECK(abs(res3[0].t - 6.80798) < 0.0001);
    CHECK(abs(res3[1].t - 7.08872) < 0.0001);
  }
  SUBCASE("Normal vector on a cone") {
    auto cone = RayTracerChallenge::Cylinder::create();
    CHECK(cone->localNormalAt({1.0, 0.0, 0.0, 1.0})
          == RayTracerChallenge::Tuple(1.0, 0.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.0, 5.0, -1.0, 1.0})
          == RayTracerChallenge::Tuple(0.0, 0.0, -1.0, 0.0));
    CHECK(cone->localNormalAt({0.0, -2.0, 1.0, 1.0})
          == RayTracerChallenge::Tuple(0.0, 0.0, 1.0, 0.0));
    CHECK(cone->localNormalAt({-1.0, 1.0, 0.0, 1.0})
          == RayTracerChallenge::Tuple(-1.0, 0.0, 0.0, 0.0));
  }
  SUBCASE("Truncated cones") {
    auto cone = RayTracerChallenge::Cylinder::create(1.0, 2.0);
    auto res1
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 1.5, 0.0, 1.0),
                                      RayTracerChallenge::Tuple(0.1, 1.0, 1.0, 0.0).normalize());
    CHECK(res1.size() == 0);
    auto res2
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 3.0, -5.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, 1.0, 1.0, 0.0).normalize());
    CHECK(res2.size() == 0);
    auto res3
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 0.0, -5.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res3.size() == 0);
    auto res4
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 2.0, -5.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res4.size() == 0);
    auto res5
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 1.0, -5.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res5.size() == 0);
    auto res6
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 1.5, -2.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res6.size() == 2);
  }
  SUBCASE("Intersecting a cones and end caps") {
    auto cone = RayTracerChallenge::Cylinder::create(1.0, 2.0, true);
    auto res1
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 3.0, 0.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, -1.0, 0.0, 0.0).normalize());
    CHECK(res1.size() == 2);
    auto res2
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 3.0, -2.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, -1.0, 2.0, 0.0).normalize());
    CHECK(res2.size() == 2);
    auto res3
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 4.0, -2.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, -1.0, 1.0, 0.0).normalize());
    CHECK(res3.size() == 2);
    auto res4
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, 0.0, -2.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, 1.0, 2.0, 0.0).normalize());
    CHECK(res4.size() == 2);
    auto res5
        = getIntersectionsForCylinder(cone, RayTracerChallenge::Tuple(0.0, -1.0, -2.0, 1.0),
                                      RayTracerChallenge::Tuple(0.0, 1.0, 1.0, 0.0).normalize());
    CHECK(res5.size() == 2);
  }
  SUBCASE("The normal vector on a cone's end caps") {
    auto cone = RayTracerChallenge::Cylinder::create(1.0, 2.0, true);
    CHECK(cone->localNormalAt({0.0, 1.0, 0.0, 1.0})
          == RayTracerChallenge::Tuple(0.0, -1.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.5, 1.0, 0.0, 1.0})
          == RayTracerChallenge::Tuple(0.0, -1.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.0, 1.0, 0.5, 1.0})
          == RayTracerChallenge::Tuple(0.0, -1.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.0, 2.0, 0.0, 1.0})
          == RayTracerChallenge::Tuple(0.0, 1.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.5, 2.0, 0.0, 1.0})
          == RayTracerChallenge::Tuple(0.0, 1.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({0.0, 2.0, 0.5, 1.0})
          == RayTracerChallenge::Tuple(0.0, 1.0, 0.0, 0.0));
  }
}
static RayTracerChallenge::Intersections getIntersectionsForCone(
    const std::shared_ptr<RayTracerChallenge::Shape>& cone, RayTracerChallenge::Tuple origin,
    RayTracerChallenge::Tuple direction) {
  auto r = RayTracerChallenge::Ray(origin, direction);
  return cone->localIntersect(r);
}
TEST_CASE("Cones") {
  SUBCASE("Intersecting a cone with a ray") {
    auto cone = RayTracerChallenge::Cone::create();
    auto res1 = getIntersectionsForCone(cone, RayTracerChallenge::Tuple(0.0, 0.0, -5.0, 1.0),
                                        RayTracerChallenge::Tuple(0.0, 0.0, 1.0, 0.0).normalize());
    CHECK(res1[0].t == 5.0);
    CHECK(res1[1].t == 5.0);
    auto res2 = getIntersectionsForCone(cone, RayTracerChallenge::Tuple(0.0, 0.0, -5.0, 1.0),
                                        RayTracerChallenge::Tuple(1.0, 1.0, 1.0, 0.0).normalize());
    CHECK(abs(res2[0].t - 8.66025) < 0.0001);
    CHECK(abs(res2[1].t - 8.66025) < 0.0001);
    auto res3
        = getIntersectionsForCone(cone, RayTracerChallenge::Tuple(1.0, 1.0, -5.0, 1.0),
                                  RayTracerChallenge::Tuple(-0.5, -1.0, 1.0, 0.0).normalize());
    CHECK(abs(res3[0].t - 4.55006) < 0.0001);
    CHECK(abs(res3[1].t - 49.44994) < 0.0001);
    auto res4 = getIntersectionsForCone(cone, RayTracerChallenge::Tuple(0.0, 0.0, -1.0, 1.0),
                                        RayTracerChallenge::Tuple(0.0, 1.0, 1.0, 0.0).normalize());
    CHECK(abs(res4[0].t - 0.35355) < 0.0001);
  }
  SUBCASE("Intersecting a cone's end caps") {
    auto cone = RayTracerChallenge::Cone::create(-0.5, 0.5, true);
    auto res1 = getIntersectionsForCone(cone, RayTracerChallenge::Tuple(0.0, 0.0, -5.0, 1.0),
                                        RayTracerChallenge::Tuple(0.0, 1.0, 0.0, 0.0).normalize());
    CHECK(res1.size() == 0);
    auto res2 = getIntersectionsForCone(cone, RayTracerChallenge::Tuple(0.0, 0.0, -0.25, 1.0),
                                        RayTracerChallenge::Tuple(0.0, 1.0, 1.0, 0.0).normalize());
    CHECK(res2.size() == 2);
    auto res3 = getIntersectionsForCone(cone, RayTracerChallenge::Tuple(0.0, 0.0, -0.25, 1.0),
                                        RayTracerChallenge::Tuple(0.0, 1.0, 0.0, 0.0).normalize());
    CHECK(res3.size() == 4);
  }
  SUBCASE("Computing the normal vector on a cone") {
    auto cone = RayTracerChallenge::Cone::create();
    CHECK(cone->localNormalAt({0.0, 0.0, 0.0, 1.0})
          == RayTracerChallenge::Tuple(0.0, 0.0, 0.0, 0.0));
    CHECK(cone->localNormalAt({1.0, 1.0, 1.0, 1.0})
          == RayTracerChallenge::Tuple(1.0, -sqrt(2.0), 1.0, 0.0));
    CHECK(cone->localNormalAt({-1.0, -1.0, 0.0, 1.0})
          == RayTracerChallenge::Tuple(-1.0, 1.0, 0.0, 0.0));
  }
}
TEST_CASE("Groups") {
  SUBCASE("Creating a new group") {
    auto g = RayTracerChallenge::Group::create();
    CHECK(g->transform == RayTracerChallenge::Matrix::identity(4));
  }
  SUBCASE("Adding a child to a group") {
    auto gr = RayTracerChallenge::Group::create();
    auto s = RayTracerChallenge::Sphere::create();
    auto g = std::dynamic_pointer_cast<RayTracerChallenge::Group>(gr);
    g->add(s);
    CHECK(g->objects[0] == s);
    CHECK(s->parent == gr);
  }
  SUBCASE("Intersecting a ray with an empty group") {
    auto g = RayTracerChallenge::Group::create();
    auto r = RayTracerChallenge::Ray({0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = g->localIntersect(r);
    CHECK(xs.size() == 0);
  }
  SUBCASE("Intersecting a ray with a non-empty group") {
    auto g = RayTracerChallenge::Group::create();
    auto s1 = RayTracerChallenge::Sphere::create();
    auto s2 = RayTracerChallenge::Sphere::create();
    auto s3 = RayTracerChallenge::Sphere::create();
    s2->transform = s2->transform.translated(0.0, 0.0, -3.0);
    s3->transform = s3->transform.translated(5.0, 0.0, 0.0);
    std::dynamic_pointer_cast<RayTracerChallenge::Group>(g)->add(s1);
    std::dynamic_pointer_cast<RayTracerChallenge::Group>(g)->add(s2);
    std::dynamic_pointer_cast<RayTracerChallenge::Group>(g)->add(s3);
    auto r = RayTracerChallenge::Ray({0.0, 0.0, -5.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = g->localIntersect(r);
    CHECK(xs[0].object == s2);
    CHECK(xs[1].object == s2);
    CHECK(xs[2].object == s1);
    CHECK(xs[3].object == s1);
  }
  SUBCASE("Intersecting a transformed group") {
    auto g = RayTracerChallenge::Group::create();
    g->transform = g->transform.scaled(2.0, 2.0, 2.0);
    auto s = RayTracerChallenge::Sphere::create();
    s->transform = s->transform.translation(5.0, 0.0, 0.0);
    std::dynamic_pointer_cast<RayTracerChallenge::Group>(g)->add(s);
    auto r = RayTracerChallenge::Ray({10.0, 0.0, -10.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = g->intersect(r);
    CHECK(xs.size() == 2);
  }
  SUBCASE("Converting a point from world to object space") {
    auto g1 = RayTracerChallenge::Group::create();
    g1->transform = g1->transform.rotatedY(M_PI / 2.0);
    auto g2 = RayTracerChallenge::Group::create();
    g2->transform = g2->transform.scaled(2.0, 2.0, 2.0);
    std::dynamic_pointer_cast<RayTracerChallenge::Group>(g1)->add(g2);
    auto s = RayTracerChallenge::Sphere::create();
    s->transform = s->transform.translation(5.0, 0.0, 0.0);
    std::dynamic_pointer_cast<RayTracerChallenge::Group>(g2)->add(s);
    auto p = s->worldToObject({-2.0, 0.0, -10.0, 1.0});
    CHECK(p == RayTracerChallenge::Tuple({0.0, 0.0, -1.0, 1.0}));
  }
  SUBCASE("Converting a normal from object to world space") {
    auto g1 = RayTracerChallenge::Group::create();
    g1->transform = g1->transform.rotatedY(M_PI / 2.0);
    auto g2 = RayTracerChallenge::Group::create();
    g2->transform = g2->transform.scaled(1.0, 2.0, 3.0);
    std::dynamic_pointer_cast<RayTracerChallenge::Group>(g1)->add(g2);
    auto s = RayTracerChallenge::Sphere::create();
    s->transform = s->transform.translation(5.0, 0.0, 0.0);
    std::dynamic_pointer_cast<RayTracerChallenge::Group>(g2)->add(s);
    auto n = s->normalToWorld({sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, 0.0});
    CHECK(n == RayTracerChallenge::Tuple(0.2857, 0.4286, -0.8571, 0.0));
  }
  SUBCASE("Finding the normal on a child object") {
    auto g1 = RayTracerChallenge::Group::create();
    g1->transform = g1->transform.rotatedY(M_PI / 2.0);
    auto g2 = RayTracerChallenge::Group::create();
    g2->transform = g2->transform.scaled(1.0, 2.0, 3.0);
    std::dynamic_pointer_cast<RayTracerChallenge::Group>(g1)->add(g2);
    auto s = RayTracerChallenge::Sphere::create();
    s->transform = s->transform.translation(5.0, 0.0, 0.0);
    std::dynamic_pointer_cast<RayTracerChallenge::Group>(g2)->add(s);
    auto n = s->normalAt({1.7321, 1.1547, -5.5774, 1.0});
    CHECK(n == RayTracerChallenge::Tuple(0.2857, 0.4286, -0.8571, 0.0));
  }
}
TEST_CASE("Bounding boxes") {
  SUBCASE("Creating a bounding box with volume") {
    auto b = RayTracerChallenge::BoundingBox({-1.0, -2.0, -3.0, 1.0}, {3.0, 2.0, 1.0, 1.0});
    CHECK(b.min == RayTracerChallenge::Tuple(-1.0, -2.0, -3.0, 1.0));
    CHECK(b.max == RayTracerChallenge::Tuple(3.0, 2.0, 1.0, 1.0));
  }
  SUBCASE("Adding points to an empty bounding box") {
    auto b = RayTracerChallenge::BoundingBox();
    b.add({-5.0, 2.0, 0.0, 1.0});
    b.add({7.0, 0.0, -3.0, 1.0});
    CHECK(b.min == RayTracerChallenge::Tuple(-5.0, -0.0, -3.0, 1.0));
    CHECK(b.max == RayTracerChallenge::Tuple(7.0, 2.0, 0.0, 1.0));
  }
  SUBCASE("A sphere has a bounding box") {
    auto sphere = RayTracerChallenge::Sphere::create();
    auto b = sphere->bounds();
    CHECK(b.min == RayTracerChallenge::Tuple(-1.0, -1.0, -1.0, 1.0));
    CHECK(b.max == RayTracerChallenge::Tuple(1.0, 1.0, 1.0, 1.0));
  }
  SUBCASE("A plane has a bounding box") {
    auto plane = RayTracerChallenge::Plane::create();
    auto b = plane->bounds();
    CHECK(b.min == RayTracerChallenge::Tuple(-INFINITY, 0.0, -INFINITY, 1.0));
    CHECK(b.max == RayTracerChallenge::Tuple(INFINITY, 0.0, INFINITY, 1.0));
  }
  SUBCASE("A cube has a bounding box") {
    auto cube = RayTracerChallenge::Cube::create();
    auto b = cube->bounds();
    CHECK(b.min == RayTracerChallenge::Tuple(-1.0, -1.0, -1.0, 1.0));
    CHECK(b.max == RayTracerChallenge::Tuple(1.0, 1.0, 1.0, 1.0));
  }
  SUBCASE("An unbounded cylinder has a bounding box") {
    auto cyl = RayTracerChallenge::Cylinder::create();
    auto b = cyl->bounds();
    CHECK(b.min == RayTracerChallenge::Tuple(-1.0, -INFINITY, -1.0, 1.0));
    CHECK(b.max == RayTracerChallenge::Tuple(1.0, INFINITY, 1.0, 1.0));
  }
  SUBCASE("A bounded cylinder has a bounding box") {
    auto cyl = RayTracerChallenge::Cylinder::create(-5.0, 3.0, true);
    auto b = cyl->bounds();
    CHECK(b.min == RayTracerChallenge::Tuple(-1.0, -5.0, -1.0, 1.0));
    CHECK(b.max == RayTracerChallenge::Tuple(1.0, 3.0, 1.0, 1.0));
  }
  SUBCASE("An unbounded cone has a bounding box") {
    auto cone = RayTracerChallenge::Cone::create();
    auto b = cone->bounds();
    CHECK(b.min == RayTracerChallenge::Tuple(-INFINITY, -INFINITY, -INFINITY, 1.0));
    CHECK(b.max == RayTracerChallenge::Tuple(INFINITY, INFINITY, INFINITY, 1.0));
  }
  SUBCASE("A bounded cone has a bounding box") {
    auto cone = RayTracerChallenge::Cone::create(-5.0, 3.0, true);
    auto b = cone->bounds();
    CHECK(b.min == RayTracerChallenge::Tuple(-5.0, -5.0, -5.0, 1.0));
    CHECK(b.max == RayTracerChallenge::Tuple(5.0, 3.0, 5.0, 1.0));
  }
  SUBCASE("Adding one bounding box to another") {
    auto box1 = RayTracerChallenge::BoundingBox({-5.0, -2.0, 0.0, 1.0}, {7.0, 4.0, 4.0, 1.0});
    auto box2 = RayTracerChallenge::BoundingBox({8.0, -7.0, -2.0, 1.0}, {14.0, 2.0, 8.0, 1.0});
    box1.add(box2);
    CHECK(box1.min == RayTracerChallenge::Tuple(-5.0, -7.0, -2.0, 1.0));
    CHECK(box1.max == RayTracerChallenge::Tuple(14.0, 4.0, 8.0, 1.0));
  }
  SUBCASE("Checking to see if a box contains a given point") {
    auto box = RayTracerChallenge::BoundingBox({5.0, -2.0, 0.0, 1.0}, {11.0, 4.0, 7.0, 1.0});
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
    auto box1 = RayTracerChallenge::BoundingBox({5.0, -2.0, 0.0, 1.0}, {11.0, 4.0, 7.0, 1.0});
    CHECK(box1.contains({{5.0, -2.0, 0.0, 1.0}, {11.0, 4.0, 7.0, 1.0}}));
    CHECK(box1.contains({{6.0, -1.0, 1.0, 1.0}, {10.0, 3.0, 6.0, 1.0}}));
    CHECK(!box1.contains({{4.0, -3.0, 0 - 10, 1.0}, {10.0, 3.0, 6.0, 1.0}}));
    CHECK(!box1.contains({{6.0, -1.0, 1.0, 1.0}, {12.0, 5.0, 8.0, 1.0}}));
  }
  SUBCASE("Transforming bounding boxes") {
    auto box = RayTracerChallenge::BoundingBox({-1.0, -1.0, -1.0, 1.0}, {1.0, 1.0, 1.0, 1.0});
    auto matrix = RayTracerChallenge::Matrix::rotationX(M_PI / 4.0)
                  * RayTracerChallenge::Matrix::rotationY(M_PI / 4.0);
    auto box2 = box.transform(matrix);
    CHECK(box2.min == RayTracerChallenge::Tuple(-1.4142, -1.7071, -1.7071, 1.0));
    CHECK(box2.max == RayTracerChallenge::Tuple(1.4142, 1.7071, 1.7071, 1.0));
  }
  SUBCASE("Querying a shape's bounding box in its parent's space") {
    auto shape = RayTracerChallenge::Sphere::create();
    shape->transform = shape->transform.scaled(0.5, 2.0, 4.0).translated(1.0, -3.0, 5.0);
    auto box = shape->parentSpaceBounds();
    CHECK(box.min == RayTracerChallenge::Tuple(0.5, -5.0, 1.0, 1.0));
    CHECK(box.max == RayTracerChallenge::Tuple(1.5, -1.0, 9.0, 1.0));
  }
  SUBCASE("A group has a bounding box which contains its children") {
    auto s = RayTracerChallenge::Sphere::create();
    s->transform = s->transform.scaled(2.0, 2.0, 2.0).translated(2.0, 5.0, -3.0);
    auto cyl = RayTracerChallenge::Cylinder::create(-2.0, 2.0, true);
    cyl->transform = cyl->transform.scaled(0.5, 1.0, 0.5).translated(-4.0, -1.0, 4.0);
    auto shape = RayTracerChallenge::Group();
    shape.add(s);
    shape.add(cyl);
    auto box = shape.bounds();
    CHECK(box.min == RayTracerChallenge::Tuple(-4.5, -3.0, -5.0, 1.0));
    CHECK(box.max == RayTracerChallenge::Tuple(4.0, 7.0, 4.5, 1.0));
  }
  SUBCASE("Intersecting a ray with a bounding box at the origin") {
    auto box = RayTracerChallenge::BoundingBox({-1.0, -1.0, -1.0, 1.0}, {1.0, 1.0, 1.0, 1.0});
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
    auto box = RayTracerChallenge::BoundingBox({5.0, -2.0, 0.0, 1.0}, {11.0, 4.0, 7.0, 1.0});
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
}

TEST_CASE("Triangles") {
  SUBCASE("Normal vector for a triangle") {
    auto t = RayTracerChallenge::Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0},
                                                  {1.0, 0.0, 0.0, 1.0});
    auto n1 = t->localNormalAt({0.0, 0.5, 0.0, 1.0});
    auto n2 = t->localNormalAt({-0.5, 0.75, 0.0, 1.0});
    auto n3 = t->localNormalAt({0.5, 0.25, 0.0, 1.0});
    auto normal = std::dynamic_pointer_cast<RayTracerChallenge::Triangle>(t)->normal;
    CHECK(n1 == normal);
    CHECK(n2 == normal);
    CHECK(n3 == normal);
  }
  SUBCASE("Intersecting a ray parallel to the triangle") {
    auto t = RayTracerChallenge::Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0},
                                                  {1.0, 0.0, 0.0, 1.0});
    auto r = RayTracerChallenge::Ray({0.0, -1.0, -2.0, 1.0}, {0.0, 1.0, 0.0, 0.0});
    CHECK(t->localIntersect(r).size() == 0);
  }
  SUBCASE("A ray misses the p1-p3 edge") {
    auto t = RayTracerChallenge::Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0},
                                                  {1.0, 0.0, 0.0, 1.0});
    auto r = RayTracerChallenge::Ray({1.0, 1.0, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    CHECK(t->localIntersect(r).size() == 0);
  }
  SUBCASE("A ray misses the p1-p2 edge") {
    auto t = RayTracerChallenge::Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0},
                                                  {1.0, 0.0, 0.0, 1.0});
    auto r = RayTracerChallenge::Ray({-1.0, 1.0, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    CHECK(t->localIntersect(r).size() == 0);
  }
  SUBCASE("A ray misses the p2-p3 edge") {
    auto t = RayTracerChallenge::Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0},
                                                  {1.0, 0.0, 0.0, 1.0});
    auto r = RayTracerChallenge::Ray({0.0, -1.0, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    CHECK(t->localIntersect(r).size() == 0);
  }
  SUBCASE("A ray strikes a triangle") {
    auto t = RayTracerChallenge::Triangle::create({0.0, 1.0, 0.0, 1.0}, {-1.0, 0.0, 0.0, 1.0},
                                                  {1.0, 0.0, 0.0, 1.0});
    auto r = RayTracerChallenge::Ray({0.0, 0.5, -2.0, 1.0}, {0.0, 0.0, 1.0, 0.0});
    auto xs = t->localIntersect(r);
    CHECK(xs.size() == 1);
    CHECK(xs[0].t == 2.0);
  }
}