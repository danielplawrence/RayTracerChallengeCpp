#include <doctest/doctest.h>
#include <raytracerchallenge/raytracerchallenge.h>

#include <cmath>
#include <sstream>
#include <string>

TEST_CASE("Tuples") {
  using namespace raytracerchallenge;
  SUBCASE("The default tuple initializes with all values set to zero") {
    RayTracerChallenge::Tuple tuple(0, 0, 0, 0);
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

TEST_CASE("Matrices") {
  using namespace raytracerchallenge;
  SUBCASE("Constructing and inspecting a 4x4 matrix") {
    std::vector<std::vector<float>> vect{
        {1.0f, 2.0f, 3.0f, 4.0f},
        {5.5f, 6.5f, 7.5f, 8.5f},
        {9.0f, 10.0f, 11.0f, 12.0f},
        {13.5f, 14.5f, 15.5f, 16.5f},
    };
    RayTracerChallenge::Matrix matrix(4, 4, vect);
    CHECK(matrix[0][0] == 1.0f);
    CHECK(matrix[0][3] == 4.0f);
    CHECK(matrix[1][0] == 5.5f);
    CHECK(matrix[1][2] == 7.5f);
    CHECK(matrix[2][2] == 11.0f);
    CHECK(matrix[3][0] == 13.5f);
    CHECK(matrix[3][2] == 15.5f);
  }
  SUBCASE("A 2x2 matrix ought to be representable") {
    std::vector<std::vector<float>> vect{{-3.0f, 5.0f}, {1.0f, -2.0f}};
    RayTracerChallenge::Matrix matrix(2, 2, vect);
    CHECK(matrix[0][0] == -3.0f);
    CHECK(matrix[0][1] == 5.0f);
    CHECK(matrix[1][0] == 1.0f);
    CHECK(matrix[1][1] == -2.0f);
  }
  SUBCASE("A 3x3 matrix ought to be representable") {
    std::vector<std::vector<float>> vect{
        {-3.0f, 5.0f, 0.0f}, {1.0f, -2.0f, -7.0f}, {0.0f, 1.0f, 1.0f}};
    RayTracerChallenge::Matrix matrix(3, 3, vect);
    CHECK(matrix[0][0] == -3.0f);
    CHECK(matrix[1][1] == -2.0f);
    CHECK(matrix[2][2] == 1.0f);
  }
  SUBCASE("Matrix equality with identical matrices") {
    std::vector<std::vector<float>> vect1{{1.0f, 2.0f, 3.0f, 4.0f},
                                          {5.0f, 6.0f, 7.0f, 8.0f},
                                          {9.0f, 8.0f, 7.0f, 6.0f},
                                          {5.0f, 4.0f, 3.0f, 2.0f}};
    std::vector<std::vector<float>> vect2{{1.0f, 2.0f, 3.0f, 4.0f},
                                          {5.0f, 6.0f, 7.0f, 8.0f},
                                          {9.0f, 8.0f, 7.0f, 6.0f},
                                          {5.0f, 4.0f, 3.0f, 2.0f}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK(matrix2 == matrix1);
  }
  SUBCASE("Matrix equality with different matrices") {
    std::vector<std::vector<float>> vect1{{1.0f, 2.0f, 3.0f, 4.0f},
                                          {5.5f, 6.0f, 7.0f, 8.0f},
                                          {9.0f, 8.0f, 7.0f, 6.0f},
                                          {5.0f, 4.0f, 3.0f, 2.0f}};
    std::vector<std::vector<float>> vect2{{2.0f, 3.0f, 4.0f, 5.0f},
                                          {6.0f, 7.0f, 8.0f, 9.0f},
                                          {8.0f, 7.0f, 6.0f, 5.0f},
                                          {4.0f, 3.0f, 2.0f, 1.0f}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK(matrix2 != matrix1);
  }
  SUBCASE("Multiplying two matrices") {
    std::vector<std::vector<float>> vect1{{1.0f, 2.0f, 3.0f, 4.0f},
                                          {5.0f, 6.0f, 7.0f, 8.0f},
                                          {9.0f, 8.0f, 7.0f, 6.0f},
                                          {5.0f, 4.0f, 3.0f, 2.0f}};
    std::vector<std::vector<float>> vect2{{-2.0f, 1.0f, 2.0f, 3.0f},
                                          {3.0f, 2.0f, 1.0f, -1.0f},
                                          {4.0f, 3.0f, 6.0f, 5.0f},
                                          {1.0f, 2.0f, 7.0f, 8.0f}};
    std::vector<std::vector<float>> vect3{{20.0f, 22.0f, 50.0f, 48.0f},
                                          {44.0f, 54.0f, 114.0f, 108.0f},
                                          {40.0f, 58.0f, 110.0f, 102.0f},
                                          {16.0f, 26.0f, 46.0f, 42.0f}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    RayTracerChallenge::Matrix matrix3(4, 4, vect3);
    CHECK(matrix1 * matrix2 == matrix3);
  }
  SUBCASE("A matrix multiplied by a tuple") {
    std::vector<std::vector<float>> vect1{{1.0f, 2.0f, 3.0f, 4.0f},
                                          {2.0f, 4.0f, 4.0f, 2.0f},
                                          {8.0f, 6.0f, 4.0f, 1.0f},
                                          {0.0f, 0.0f, 0.0f, 1.0f}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Tuple tuple(1, 2, 3, 1);
    CHECK(matrix1 * tuple == RayTracerChallenge::Tuple(18, 24, 33, 1));
  }
  SUBCASE("The identity matrix") {
    std::vector<std::vector<float>> identity{{1.0f, 0.0f, 0.0f, 0.0f},
                                             {0.0f, 1.0f, 0.0f, 0.0f},
                                             {0.0f, 0.0f, 1.0f, 0.0f},
                                             {0.0f, 0.0f, 0.0f, 1.0f}};
    RayTracerChallenge::Matrix matrix1(4, 4, identity);
    RayTracerChallenge::Matrix matrix2 = RayTracerChallenge::Matrix::identity(4);
    CHECK(matrix2 == matrix1);
  }
  SUBCASE("Multiplying a matrix by the identity matrix") {
    std::vector<std::vector<float>> vect1{{0.0f, 0.0f, 2.0f, 4.0f},
                                          {1.0f, 2.0f, 4.0f, 8.0f},
                                          {2.0f, 4.0f, 8.0f, 16.0f},
                                          {4.0f, 8.0f, 16.0f, 32.0f}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2 = RayTracerChallenge::Matrix::identity(4);
    CHECK(matrix1 * matrix2 == matrix1);
  }
  SUBCASE("Transposing a matrix") {
    std::vector<std::vector<float>> vect1{{0.0f, 9.0f, 3.0f, 0.0f},
                                          {9.0f, 8.0f, 0.0f, 8.0f},
                                          {1.0f, 8.0f, 5.0f, 3.0f},
                                          {0.0f, 0.0f, 5.0f, 8.0f}};
    std::vector<std::vector<float>> vect2{{0.0f, 9.0f, 1.0f, 0.0f},
                                          {9.0f, 8.0f, 8.0f, 0.0f},
                                          {3.0f, 0.0f, 5.0f, 5.0f},
                                          {0.0f, 8.0f, 3.0f, 8.0f}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK(matrix1.transposed() == matrix2);
  }
  SUBCASE("Transposing the identity matrix") {
    RayTracerChallenge::Matrix matrix1 = RayTracerChallenge::Matrix::identity(4);
    CHECK(matrix1.transposed() == matrix1);
  }
  SUBCASE("Calculating the determinant of a 2x2 matrix") {
    std::vector<std::vector<float>> vect1{
        {1.0f, 5.0f},
        {-3.0f, 2.0f},
    };
    RayTracerChallenge::Matrix matrix1(2, 2, vect1);
    CHECK(matrix1.determinant() == 17.0f);
  }
  SUBCASE("A submatrix of a 3x3 matrix is a 2x2 matrix") {
    std::vector<std::vector<float>> vect1{
        {1.0f, 5.0f, 0.0f}, {-3.0f, 2.0f, 7.0f}, {0.0f, 6.0f, -3.0f}};
    std::vector<std::vector<float>> vect2{{-3.0f, 2.0f}, {0.0f, 6.0f}};
    RayTracerChallenge::Matrix matrix1(3, 3, vect1);
    RayTracerChallenge::Matrix matrix2(2, 2, vect2);
    CHECK(matrix1.submatrix(0, 2) == matrix2);
  }
  SUBCASE("A submatrix of a 4x4 matrix is a 3x3 matrix") {
    std::vector<std::vector<float>> vect1{{-6.0f, 1.0f, 1.0f, 6.0f},
                                          {-8.0f, 5.0f, 8.0f, 6.0f},
                                          {-1.0f, 0.0f, 8.0f, 2.0f},
                                          {-7.0f, 1.0f, -1.0f, 1.0f}};
    std::vector<std::vector<float>> vect2{
        {-6.0f, 1.0f, 6.0f}, {-8.0f, 8.0f, 6.0f}, {-7.0f, -1.0f, 1.0f}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(3, 3, vect2);
    CHECK(matrix1.submatrix(2, 1) == matrix2);
  }
  SUBCASE("Calculating the minor of a 3x3 matrix") {
    std::vector<std::vector<float>> vect{
        {-3.0f, 5.0f, 0.0f}, {2.0f, -1.0f, -7.0f}, {6.0f, -1.0f, 5.0f}};
    RayTracerChallenge::Matrix matrix(3, 3, vect);
    CHECK(matrix.minor(1, 0) == 25);
    CHECK(matrix.minor(1, 0) == matrix.submatrix(1, 0).determinant());
  }
  SUBCASE("Calculating the cofactor of a 3x3 matrix") {
    std::vector<std::vector<float>> vect{
        {-3.0f, 5.0f, 0.0f}, {2.0f, -1.0f, -7.0f}, {6.0f, -1.0f, 5.0f}};
    RayTracerChallenge::Matrix matrix(3, 3, vect);
    CHECK(matrix.cofactor(0, 0) == -12.0f);
    CHECK(matrix.cofactor(1, 0) == -25.0f);
  }
  SUBCASE("Calculating the determinant of a 3x3 matrix") {
    std::vector<std::vector<float>> vect{
        {1.0f, 2.0f, 6.0f}, {-5.0f, 8.0f, -4.0f}, {2.0f, 6.0f, 4.0f}};
    RayTracerChallenge::Matrix matrix(3, 3, vect);
    CHECK(matrix.cofactor(0, 0) == 56.0f);
    CHECK(matrix.cofactor(0, 1) == 12.0f);
    CHECK(matrix.cofactor(0, 2) == -46.0f);
    CHECK(matrix.determinant() == -196.0f);
  }
  SUBCASE("Calculating the determinant of a 4x4 matrix") {
    std::vector<std::vector<float>> vect{{-2.0f, -8.0f, 3.0f, 5.0f},
                                         {-3.0f, 1.0f, 7.0f, 3.0f},
                                         {1.0f, 2.0f, -9.0f, 6.0f},
                                         {-6.0f, 7.0f, 7.0f, -9.0f}};
    RayTracerChallenge::Matrix matrix(4, 4, vect);
    CHECK(matrix.cofactor(0, 0) == 690.0f);
    CHECK(matrix.cofactor(0, 1) == 447.0f);
    CHECK(matrix.cofactor(0, 2) == 210.0f);
    CHECK(matrix.cofactor(0, 3) == 51.0f);
    CHECK(matrix.determinant() == -4071.0f);
  }
  SUBCASE("Testing an invertible matrix for invertibility") {
    std::vector<std::vector<float>> vect{{6.0f, 4.0f, 4.0f, 4.0f},
                                         {5.0f, 5.0f, 7.0f, 6.0f},
                                         {4.0f, -9.0f, 3.0f, -7.0f},
                                         {9.0f, 1.0f, 7.0f, -6.0f}};
    RayTracerChallenge::Matrix matrix(4, 4, vect);
    CHECK(matrix.invertible() == true);
  }
  SUBCASE("Testing an noninvertible matrix for invertibility") {
    std::vector<std::vector<float>> vect{{-4.0f, 2.0f, -2.0f, -3.0f},
                                         {9.0f, 6.0f, 2.0f, 6.0f},
                                         {0.0f, -5.0f, 1.0f, -5.0f},
                                         {0.0f, 0.0f, 0.0f, 0.0f}};
    RayTracerChallenge::Matrix matrix(4, 4, vect);
    CHECK(matrix.invertible() == false);
  }
  SUBCASE("Calculating the inverse of a matrix") {
    std::vector<std::vector<float>> vect1{{-5.0f, 2.0f, 6.0f, -8.0f},
                                          {1.0f, -5.0f, 1.0f, 8.0f},
                                          {7.0f, 7.0f, -6.0f, -7.0f},
                                          {1.0f, -3.0f, 7.0f, 4.0f}};
    std::vector<std::vector<float>> vect2{{0.21805f, 0.45113f, 0.24060f, -0.04511f},
                                          {-0.80827f, -1.45677f, -0.44361f, 0.52068f},
                                          {-0.07895f, -0.22368f, -0.05263f, 0.19737f},
                                          {-0.52256f, -0.81391f, -0.30075f, 0.30639f}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK(matrix1.inverse() == matrix2);
  }
  SUBCASE("Calculating the inverse of another matrix") {
    std::vector<std::vector<float>> vect1{{8.0f, -5.0f, 9.0f, 2.0f},
                                          {7.0f, 5.0f, 6.0f, 1.0f},
                                          {-6.0f, 0.0f, 9.0f, 6.0f},
                                          {-3.0f, 0.0f, -9.0f, -4.0f}};
    std::vector<std::vector<float>> vect2{{-0.15385f, -0.15385f, -0.28205f, -0.53846f},
                                          {-0.07692f, 0.12308f, 0.02564f, 0.03077f},
                                          {0.35897f, 0.35897f, 0.43590f, 0.92308f},
                                          {-0.69231f, -0.69231f, -0.76932f, -1.92308f}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK(matrix1.inverse() == matrix2);
  }
  SUBCASE("Calculating the inverse of a third matrix") {
    std::vector<std::vector<float>> vect1{{9.0f, 3.0f, 0.0f, 9.0f},
                                          {-5.0f, -2.0f, -6.0f, -3.0f},
                                          {-4.0f, 9.0f, 6.0f, 4.0f},
                                          {-7.0f, 6.0f, 6.0f, 2.0f}};
    std::vector<std::vector<float>> vect2{{-0.04074f, -0.07778f, 0.14444f, -0.22222f},
                                          {-0.07778f, 0.03333f, 0.36667f, -0.33333f},
                                          {-0.02901f, -0.14630f, -0.10926f, 0.12963f},
                                          {0.17778f, 0.06667f, -0.26667f, 0.33333f}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    CHECK(matrix1.inverse() == matrix2);
  }
  SUBCASE("Multiplying a product by its inverse") {
    std::vector<std::vector<float>> vect1{{3.0f, -9.0f, 7.0f, 3.0f},
                                          {3.0f, -8.0f, 2.0f, -9.0f},
                                          {-4.0f, 4.0f, 4.0f, 1.0f},
                                          {-6.0f, 5.0f, -1.0f, 1.0f}};
    std::vector<std::vector<float>> vect2{{8.0f, 2.0f, 2.0f, 2.0f},
                                          {3.0f, -1.0f, 7.0f, 0.0f},
                                          {7.0f, 0.0f, 5.0f, 4.0f},
                                          {6.0f, -2.0f, 0.0f, 5.0f}};
    RayTracerChallenge::Matrix matrix1(4, 4, vect1);
    RayTracerChallenge::Matrix matrix2(4, 4, vect2);
    RayTracerChallenge::Matrix product = matrix1 * matrix2;
    CHECK(product * matrix2.inverse() == matrix1);
  }
}