#define _USE_MATH_DEFINES
#include <doctest/doctest.h>
#include <raytracerchallenge/raytracerchallenge.h>

#include <cmath>
#include <sstream>
#include <string>

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
    CHECK(matrix[0][0] == 1.0);
    CHECK(matrix[0][3] == 4.0);
    CHECK(matrix[1][0] == 5.5);
    CHECK(matrix[1][2] == 7.5);
    CHECK(matrix[2][2] == 11.0);
    CHECK(matrix[3][0] == 13.5);
    CHECK(matrix[3][2] == 15.5);
  }
  SUBCASE("A 2x2 matrix ought to be representable") {
    std::vector<std::vector<double>> vect{{-3.0, 5.0}, {1.0, -2.0}};
    RayTracerChallenge::Matrix matrix(2, 2, vect);
    CHECK(matrix[0][0] == -3.0);
    CHECK(matrix[0][1] == 5.0);
    CHECK(matrix[1][0] == 1.0);
    CHECK(matrix[1][1] == -2.0);
  }
  SUBCASE("A 3x3 matrix ought to be representable") {
    std::vector<std::vector<double>> vect{{-3.0, 5.0, 0.0}, {1.0, -2.0, -7.0}, {0.0, 1.0, 1.0}};
    RayTracerChallenge::Matrix matrix(3, 3, vect);
    CHECK(matrix[0][0] == -3.0);
    CHECK(matrix[1][1] == -2.0);
    CHECK(matrix[2][2] == 1.0);
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
  SUBCASE("A submatrix of a 3x3 matrix is a 2x2 matrix") {
    std::vector<std::vector<double>> vect1{{1.0, 5.0, 0.0}, {-3.0, 2.0, 7.0}, {0.0, 6.0, -3.0}};
    std::vector<std::vector<double>> vect2{{-3.0, 2.0}, {0.0, 6.0}};
    RayTracerChallenge::Matrix matrix1(3, 3, vect1);
    RayTracerChallenge::Matrix matrix2(2, 2, vect2);
    CHECK(matrix1.submatrix(0, 2) == matrix2);
  }
  SUBCASE("A submatrix of a 4x4 matrix is a 3x3 matrix") {
    std::vector<std::vector<double>> vect1{{-6.0, 1.0, 1.0, 6.0},
                                           {-8.0, 5.0, 8.0, 6.0},
                                           {-1.0, 0.0, 8.0, 2.0},
                                           {-7.0, 1.0, -1.0, 1.0}};
    std::vector<std::vector<double>> vect2{{-6.0, 1.0, 6.0}, {-8.0, 8.0, 6.0}, {-7.0, -1.0, 1.0}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(3, 3, vect2);
    CHECK(matrix1.submatrix(2, 1) == matrix2);
  }
  SUBCASE("Calculating the minor of a 3x3 matrix") {
    std::vector<std::vector<double>> vect{{-3.0, 5.0, 0.0}, {2.0, -1.0, -7.0}, {6.0, -1.0, 5.0}};
    RayTracerChallenge::Matrix matrix(3, 3, vect);
    CHECK(matrix.minor(1, 0) == 25);
    CHECK(matrix.minor(1, 0) == matrix.submatrix(1, 0).determinant());
  }
  SUBCASE("Calculating the cofactor of a 3x3 matrix") {
    std::vector<std::vector<double>> vect{{-3.0, 5.0, 0.0}, {2.0, -1.0, -7.0}, {6.0, -1.0, 5.0}};
    RayTracerChallenge::Matrix matrix(3, 3, vect);
    CHECK(matrix.cofactor(0, 0) == -12.0);
    CHECK(matrix.cofactor(1, 0) == -25.0);
  }
  SUBCASE("Calculating the determinant of a 3x3 matrix") {
    std::vector<std::vector<double>> vect{{1.0, 2.0, 6.0}, {-5.0, 8.0, -4.0}, {2.0, 6.0, 4.0}};
    RayTracerChallenge::Matrix matrix(3, 3, vect);
    CHECK(matrix.cofactor(0, 0) == 56.0);
    CHECK(matrix.cofactor(0, 1) == 12.0);
    CHECK(matrix.cofactor(0, 2) == -46.0);
    CHECK(matrix.determinant() == -196.0);
  }
  SUBCASE("Calculating the determinant of a 4x4 matrix") {
    std::vector<std::vector<double>> vect{{-2.0, -8.0, 3.0, 5.0},
                                          {-3.0, 1.0, 7.0, 3.0},
                                          {1.0, 2.0, -9.0, 6.0},
                                          {-6.0, 7.0, 7.0, -9.0}};
    RayTracerChallenge::Matrix matrix(4, 4, vect);
    CHECK(matrix.cofactor(0, 0) == 690.0);
    CHECK(matrix.cofactor(0, 1) == 447.0);
    CHECK(matrix.cofactor(0, 2) == 210.0);
    CHECK(matrix.cofactor(0, 3) == 51.0);
    CHECK(matrix.determinant() == -4071.0);
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
    RayTracerChallenge::Sphere sphere1;
    RayTracerChallenge::Sphere sphere2;
    CHECK(sphere1.id != sphere2.id);
  }
  SUBCASE("A Sphere has a default material") {
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Material material;
    CHECK(sphere.material == material);
  }
  SUBCASE("A Sphere may be assigned a material") {
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Material material;
    material.ambient = 1.0;
    sphere.material = material;
    CHECK(sphere.material == material);
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
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Intersection intersection(3.5, sphere);
    CHECK(intersection.t == 3.5);
    CHECK(intersection.object == sphere);
  }
  SUBCASE("Aggregating intersections") {
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Intersection intersection1(1.0, sphere);
    RayTracerChallenge::Intersection intersection2(2.0, sphere);
    std::vector<RayTracerChallenge::Intersection> intersections{intersection1, intersection2};
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].object == sphere);
    CHECK(intersections[1].object == sphere);
  }
  SUBCASE("A ray intersects a sphere at two points") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Intersections intersections = sphere.intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == 4.0);
    CHECK(intersections[1].t == 6.0);
    CHECK(intersections[0].object == sphere);
    CHECK(intersections[1].object == sphere);
  }
  SUBCASE("A ray intersects a sphere at a tangent") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 1.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Intersections intersections = sphere.intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == 5.0);
    CHECK(intersections[1].t == 5.0);
    CHECK(intersections[0].object == sphere);
    CHECK(intersections[1].object == sphere);
  }
  SUBCASE("A ray originates inside a sphere") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Intersections intersections = sphere.intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == -1.0);
    CHECK(intersections[1].t == 1.0);
    CHECK(intersections[0].object == sphere);
    CHECK(intersections[1].object == sphere);
  }
  SUBCASE("A sphere is behind a ray") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, 5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Intersections intersections = sphere.intersect(ray);
    CHECK(intersections.size() == 2);
    CHECK(intersections[0].t == -6.0);
    CHECK(intersections[1].t == -4.0);
    CHECK(intersections[0].object == sphere);
    CHECK(intersections[1].object == sphere);
  }
  SUBCASE("The hit, when all intersections have positive t") {
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Intersection intersection1(1.0, sphere);
    RayTracerChallenge::Intersection intersection2(1.0, sphere);
    RayTracerChallenge::Intersections xs({intersection1, intersection2});
    CHECK(xs.hit() == intersection1);
  }
  SUBCASE("The hit, when some intersections have negative t") {
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Intersection intersection1(-1.0, sphere);
    RayTracerChallenge::Intersection intersection2(1.0, sphere);
    RayTracerChallenge::Intersections xs({intersection1, intersection2});
    CHECK(xs.hit() == intersection2);
  }
  SUBCASE("The hit, when all intersections have negative t") {
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Intersection intersection1(-2.0, sphere);
    RayTracerChallenge::Intersection intersection2(-1.0, sphere);
    RayTracerChallenge::Intersections xs({intersection1, intersection2});
    CHECK(xs.hit().has_value() == false);
  }
  SUBCASE("The hit is always the lowest nonnegative intersection") {
    RayTracerChallenge::Sphere sphere;
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
    RayTracerChallenge::Sphere sphere;
    CHECK(sphere.transform == RayTracerChallenge::Matrix::identity(4));
  }
  SUBCASE("Changing a sphere's transformation") {
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Matrix matrix = RayTracerChallenge::Matrix::translation(2.0, 3.0, 4.0);
    sphere.transform = matrix;
    CHECK(sphere.transform == matrix);
  }
  SUBCASE("Intersecting a scaled sphere with a ray") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Sphere sphere;
    sphere.transform = RayTracerChallenge::Matrix::scaling(2.0, 2.0, 2.0);
    RayTracerChallenge::Intersections xs = sphere.intersect(ray);
    CHECK(xs.size() == 2);
    CHECK(xs[0].t == 3.0);
    CHECK(xs[1].t == 7.0);
  }
  SUBCASE("Intersecting a translated sphere with a ray") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Sphere sphere;
    sphere.transform = RayTracerChallenge::Matrix::translation(5.0, 0.0, 0.0);
    RayTracerChallenge::Intersections xs = sphere.intersect(ray);
    CHECK(xs.size() == 0);
  }
  SUBCASE("Precomputing the state of an intersection") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Intersection intersection(4.0, sphere);
    RayTracerChallenge::Computations computations = intersection.prepareComputations(ray);
    CHECK(computations.t == intersection.t);
    CHECK(computations.object.is(intersection.object));
    CHECK(computations.point == RayTracerChallenge::Tuple::point(0.0, 0.0, -1.0));
    CHECK(computations.eyeVector == RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0));
    CHECK(computations.normalVector == RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0));
  }
  SUBCASE("The hit, when an intersection occurs on the outside") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Sphere sphere;
    RayTracerChallenge::Intersection intersection(4.0, sphere);
    RayTracerChallenge::Computations computations = intersection.prepareComputations(ray);
    CHECK(computations.inside == false);
  }
  SUBCASE("The hit, when an intersection occurs on the inside") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Sphere sphere;
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
    RayTracerChallenge::Sphere sphere;
    sphere.transform = RayTracerChallenge::Matrix::translation(0.0, 0.0, 1.0);
    RayTracerChallenge::Intersection intersection(5.0, sphere);
    RayTracerChallenge::Computations computations = intersection.prepareComputations(ray);
    CHECK(computations.overPoint.z < 0.0001 / 2.0);
    CHECK(computations.point.z > computations.overPoint.z);
  }
}
TEST_CASE("Normals") {
  using namespace raytracerchallenge;
  SUBCASE("The normal on a sphere at a point on the x axis") {
    RayTracerChallenge::Sphere sphere;
    auto n = sphere.normalAt(RayTracerChallenge::Tuple::point(1.0, 0.0, 0.0));
    CHECK(n == RayTracerChallenge::Tuple::vector(1.0, 0.0, 0.0));
  }
  SUBCASE("The normal on a sphere at a point on the y axis") {
    RayTracerChallenge::Sphere sphere;
    auto n = sphere.normalAt(RayTracerChallenge::Tuple::point(0.0, 1.0, 0.0));
    CHECK(n == RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0));
  }
  SUBCASE("The normal on a sphere at a point on the z axis") {
    RayTracerChallenge::Sphere sphere;
    auto n = sphere.normalAt(RayTracerChallenge::Tuple::point(0.0, 0.0, 1.0));
    CHECK(n == RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0));
  }
  SUBCASE("The normal on a sphere at a nonaxial point") {
    RayTracerChallenge::Sphere sphere;
    auto n = sphere.normalAt(
        RayTracerChallenge::Tuple::point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
    CHECK(n
          == RayTracerChallenge::Tuple::vector(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
  }
  SUBCASE("The normal is a normalized vector") {
    RayTracerChallenge::Sphere sphere;
    auto n = sphere.normalAt(
        RayTracerChallenge::Tuple::point(sqrt(3.0) / 3.0, sqrt(3.0) / 3.0, sqrt(3.0) / 3.0));
    CHECK(n == n.normalize());
  }
  SUBCASE("Computing the normal on a translated sphere") {
    RayTracerChallenge::Sphere sphere;
    sphere.transform = RayTracerChallenge::Matrix::translation(0.0, 1.0, 0.0);
    auto n = sphere.normalAt(RayTracerChallenge::Tuple::point(0.0, 1.70711, -0.70711));
    CHECK(n == RayTracerChallenge::Tuple::vector(0.0, 0.70711, -0.70711));
  }
  SUBCASE("Computing the normal on a transformed sphere") {
    RayTracerChallenge::Sphere sphere;
    sphere.transform = (RayTracerChallenge::Matrix::scaling(1.0, 0.5, 1.0)
                        * RayTracerChallenge::Matrix::rotationZ(M_PI / 5.0));
    auto n
        = sphere.normalAt(RayTracerChallenge::Tuple::point(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0));
    CHECK(n == RayTracerChallenge::Tuple::vector(0.0, 0.97014, -0.24254));
  }
}
TEST_CASE("Planes") {
  using namespace raytracerchallenge;
  SUBCASE("The normal of a plane is constant everywhere") {
    RayTracerChallenge::Plane plane;
    auto n1 = plane.localNormalAt({0.0, 0.0, 0.0, 0.0});
    auto n2 = plane.localNormalAt({10.0, 0.0, -10.0, 0.0});
    auto n3 = plane.localNormalAt({-5.0, 0.0, 150.0, 0.0});
    CHECK(n1 == RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0));
    CHECK(n2 == RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0));
    CHECK(n3 == RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0));
  }
  SUBCASE("Intersect with a ray parallel to the plane") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 10.0, 1.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Plane plane;
    auto xs = plane.localIntersect(ray);
    CHECK(xs.size() == 0);
  }
  SUBCASE("Intersect with a coplanar ray") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0);
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Plane plane;
    auto xs = plane.localIntersect(ray);
    CHECK(xs.size() == 0);
  }
  SUBCASE("A ray intersecting a plane from above") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, 1.0, 0.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, -1.0, 0.0);
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Plane plane;
    auto xs = plane.localIntersect(ray);
    CHECK(xs.size() == 1);
    CHECK(xs[0].t == 1);
    CHECK(xs[0].object == plane);
  }
  SUBCASE("A ray intersecting a plane from below") {
    RayTracerChallenge::Tuple origin = RayTracerChallenge::Tuple::point(0.0, -1.0, 0.0);
    RayTracerChallenge::Tuple direction = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
    RayTracerChallenge::Ray ray(origin, direction);
    RayTracerChallenge::Plane plane;
    auto xs = plane.localIntersect(ray);
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
    RayTracerChallenge::Sphere s;
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.0, -10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == RayTracerChallenge::Color(1.9, 1.9, 1.9));
  }
  SUBCASE("Lighting with the eye between the light and the surface, eye offset 45 degrees") {
    RayTracerChallenge::Sphere s;
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.0, -10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == RayTracerChallenge::Color(1.0, 1.0, 1.0));
  }
  SUBCASE("Lighting with the eye opposite surface, light offset 45 degrees") {
    RayTracerChallenge::Sphere s;
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 10.0, -10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == RayTracerChallenge::Color(0.7364, 0.7364, 0.7364));
  }
  SUBCASE("Lighting with eye in the path of the reflection vector") {
    RayTracerChallenge::Sphere s;
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, -sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 10.0, -10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == RayTracerChallenge::Color(1.63638, 1.63638, 1.63638));
  }
  SUBCASE("Lighting with the light behind the surface") {
    RayTracerChallenge::Sphere s;
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.0, 10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, false);
    CHECK(result == RayTracerChallenge::Color(0.1, 0.1, 0.1));
  }
  SUBCASE("Lighting with the surface in shadow") {
    RayTracerChallenge::Sphere s;
    auto position = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
    auto eyeVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto normalVector = RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0);
    auto light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.0, -10.0),
                                                RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto result = RayTracerChallenge::lighting(s, light, position, eyeVector, normalVector, true);
    CHECK(result == RayTracerChallenge::Color(0.1, 0.1, 0.1));
  }
  SUBCASE("Lighting with a pattern applied") {
    RayTracerChallenge::Sphere s;
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    s.material.pattern = &pattern;
    s.material.ambient = 1.0;
    s.material.diffuse = 0.0;
    s.material.specular = 0.0;
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
    auto sphere1 = RayTracerChallenge::Sphere();
    sphere1.material.color = RayTracerChallenge::Color(0.8, 1.0, 0.6);
    sphere1.material.diffuse = 0.7;
    sphere1.material.specular = 0.2;
    auto sphere2 = RayTracerChallenge::Sphere();
    sphere2.transform = RayTracerChallenge::Matrix::scaling(0.5, 0.5, 0.5);
    CHECK(world.light->position == RayTracerChallenge::Tuple::point(-10.0, 10.0, -10.0));
    CHECK(world.light->intensity == RayTracerChallenge::Color(1.0, 1.0, 1.0));
    CHECK(world.contains(sphere1));
    CHECK(world.contains(sphere2));
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
    auto color = world.shadeHit(computations);
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
    auto color = world.shadeHit(computations);
    CHECK(color == RayTracerChallenge::Color(0.90498, 0.90498, 0.90498));
  }
  SUBCASE("Shading an intersection in shadow") {
    auto world = RayTracerChallenge::World();
    world.light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 0.0, 10.0),
                                                 RayTracerChallenge::Color(1.0, 1.0, 1.0));
    auto sphere = RayTracerChallenge::Sphere();
    world.add(sphere);
    auto sphere2 = RayTracerChallenge::Sphere();
    sphere2.transform = RayTracerChallenge::Matrix::translation(0.0, 0.0, 10.0);
    world.add(sphere2);
    auto ray = RayTracerChallenge::Ray(RayTracerChallenge::Tuple::point(0.0, 0.0, 5.0),
                                       RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0));
    auto intersection = RayTracerChallenge::Intersection(4.0, sphere2);
    auto computations = intersection.prepareComputations(ray);
    auto color = world.shadeHit(computations);
    CHECK(color == RayTracerChallenge::Color(0.1, 0.1, 0.1));
  }
  SUBCASE("The color when a ray misses") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto ray = RayTracerChallenge::Ray(RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0),
                                       RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0));
    auto color = world.colorAt(ray);
    CHECK(color == RayTracerChallenge::Color(0.0, 0.0, 0.0));
  }
  SUBCASE("The color when a ray hits") {
    auto world = RayTracerChallenge::World::defaultWorld();
    auto ray = RayTracerChallenge::Ray(RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0),
                                       RayTracerChallenge::Tuple::vector(0.0, 0.0, 1.0));
    auto color = world.colorAt(ray);
    CHECK(color == RayTracerChallenge::Color(0.38066, 0.47583, 0.2855));
  }
  SUBCASE("The color with an intersection behind the ray") {
    auto world = RayTracerChallenge::World::defaultWorld();
    world.objects[0].material.ambient = 1.0;
    world.objects[1].material.ambient = 1.0;
    auto ray = RayTracerChallenge::Ray(RayTracerChallenge::Tuple::point(0.0, 0.0, 0.75),
                                       RayTracerChallenge::Tuple::vector(0.0, 0.0, -1.0));
    auto color = world.colorAt(ray);
    CHECK(color == world.objects[1].material.color);
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
    auto shape = RayTracerChallenge::Sphere();
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 1.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 2.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("A stripe pattern is constant in z") {
    auto shape = RayTracerChallenge::Sphere();
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 1.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 2.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("A stripe pattern alternates in x") {
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    auto shape = RayTracerChallenge::Sphere();
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {0.9, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {1.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {-0.1, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {-1.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {-1.1, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("Stripes with an object transformation") {
    auto object = RayTracerChallenge::Sphere();
    object.transform = RayTracerChallenge::Matrix::scaling(2.0, 2.0, 2.0);
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(object, {1.5, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("Stripes with an pattern transformation") {
    auto object = RayTracerChallenge::Sphere();
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    pattern.transform = RayTracerChallenge::Matrix::scaling(2.0, 2.0, 2.0);
    CHECK(pattern.colorAt(object, {1.5, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("Stripes with both an object and pattern transformation") {
    auto object = RayTracerChallenge::Sphere();
    auto pattern = RayTracerChallenge::StripePattern(RayTracerChallenge::Color::WHITE,
                                                     RayTracerChallenge::Color::BLACK);
    object.transform = RayTracerChallenge::Matrix::scaling(2.0, 2.0, 2.0);
    pattern.transform = RayTracerChallenge::Matrix::translation(0.5, 0.0, 0.0);
    CHECK(pattern.colorAt(object, {2.5, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
  }
  SUBCASE("A gradient linearly interpolates between colors") {
    auto shape = RayTracerChallenge::Sphere();
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
    auto shape = RayTracerChallenge::Sphere();
    auto pattern = RayTracerChallenge::RingPattern(RayTracerChallenge::Color::WHITE,
                                                   RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::WHITE);
    CHECK(pattern.colorAt(shape, {1.0, 0.0, 0.0, 1.0}) == RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.0, 0.0, 1.0, 1.0}) == RayTracerChallenge::Color::BLACK);
    CHECK(pattern.colorAt(shape, {0.708, 0.0, 0.708, 1.0}) == RayTracerChallenge::Color::BLACK);
  }
}