#define _USE_MATH_DEFINES
#include <doctest/doctest.h>
#include <raytracerchallenge/base/Matrix.h>
#include <raytracerchallenge/base/Tuple.h>

#include <cmath>
#include <vector>

using namespace raytracerchallenge;

TEST_CASE("Matrices") {
  SUBCASE("Constructing and inspecting a 4x4 matrix") {
    std::vector<std::vector<double>> vect{
        {1.0, 2.0, 3.0, 4.0},
        {5.5, 6.5, 7.5, 8.5},
        {9.0, 10.0, 11.0, 12.0},
        {13.5, 14.5, 15.5, 16.5},
    };
    Matrix matrix(4, 4, vect);
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
    Matrix matrix(2, 2, vect);
    CHECK(matrix.m(0, 0) == -3.0);
    CHECK(matrix.m(0, 1) == 5.0);
    CHECK(matrix.m(1, 0) == 1.0);
    CHECK(matrix.m(1, 1) == -2.0);
  }
  SUBCASE("A 3x3 matrix ought to be representable") {
    std::vector<std::vector<double>> vect{{-3.0, 5.0, 0.0}, {1.0, -2.0, -7.0}, {0.0, 1.0, 1.0}};
    Matrix matrix(3, 3, vect);
    CHECK(matrix.m(0, 0) == -3.0);
    CHECK(matrix.m(1, 1) == -2.0);
    CHECK(matrix.m(2, 2) == 1.0);
  }
  SUBCASE("Matrix equality with identical matrices") {
    std::vector<std::vector<double>> vect1{
        {1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    std::vector<std::vector<double>> vect2{
        {1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    Matrix matrix1(4, 4, vect1);
    Matrix matrix2(4, 4, vect2);
    CHECK((matrix2 == matrix1) == true);
  }
  SUBCASE("Matrix equality with different matrices") {
    std::vector<std::vector<double>> vect1{
        {1.0, 2.0, 3.0, 4.0}, {5.5, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    std::vector<std::vector<double>> vect2{
        {2.0, 3.0, 4.0, 5.0}, {6.0, 7.0, 8.0, 9.0}, {8.0, 7.0, 6.0, 5.0}, {4.0, 3.0, 2.0, 1.0}};
    Matrix matrix1(4, 4, vect1);
    Matrix matrix2(4, 4, vect2);
    CHECK((matrix2 == matrix1) == false);
  }
  SUBCASE("Matrix inequality with different matrices") {
    std::vector<std::vector<double>> vect1{
        {1.0, 2.0, 3.0, 4.0}, {5.5, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    std::vector<std::vector<double>> vect2{
        {2.0, 3.0, 4.0, 5.0}, {6.0, 7.0, 8.0, 9.0}, {8.0, 7.0, 6.0, 5.0}, {4.0, 3.0, 2.0, 1.0}};
    Matrix matrix1(4, 4, vect1);
    Matrix matrix2(4, 4, vect2);
    CHECK((matrix2 != matrix1) == true);
  }
  SUBCASE("Matrix inequality with identical matrices") {
    std::vector<std::vector<double>> vect1{
        {1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    std::vector<std::vector<double>> vect2{
        {1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 8.0, 7.0, 6.0}, {5.0, 4.0, 3.0, 2.0}};
    Matrix matrix1(4, 4, vect1);
    Matrix matrix2(4, 4, vect2);
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
    Matrix matrix1(4, 4, vect1);
    Matrix matrix2(4, 4, vect2);
    Matrix matrix3(4, 4, vect3);
    CHECK(matrix1 * matrix2 == matrix3);
  }
  SUBCASE("A matrix multiplied by a tuple") {
    std::vector<std::vector<double>> vect1{
        {1.0, 2.0, 3.0, 4.0}, {2.0, 4.0, 4.0, 2.0}, {8.0, 6.0, 4.0, 1.0}, {0.0, 0.0, 0.0, 1.0}};
    Matrix matrix1(4, 4, vect1);
    Tuple tuple(1, 2, 3, 1);
    CHECK(matrix1 * tuple == Tuple(18, 24, 33, 1));
  }
  SUBCASE("The identity matrix") {
    std::vector<std::vector<double>> identity{
        {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
    Matrix matrix1(4, 4, identity);
    Matrix matrix2 = Matrix::identity(4);
    CHECK(matrix2 == matrix1);
  }
  SUBCASE("Multiplying a matrix by the identity matrix") {
    std::vector<std::vector<double>> vect1{
        {0.0, 0.0, 2.0, 4.0}, {1.0, 2.0, 4.0, 8.0}, {2.0, 4.0, 8.0, 16.0}, {4.0, 8.0, 16.0, 32.0}};
    Matrix matrix1(4, 4, vect1);
    Matrix matrix2 = Matrix::identity(4);
    CHECK(matrix1 * matrix2 == matrix1);
  }
  SUBCASE("Transposing a matrix") {
    std::vector<std::vector<double>> vect1{
        {0.0, 9.0, 3.0, 0.0}, {9.0, 8.0, 0.0, 8.0}, {1.0, 8.0, 5.0, 3.0}, {0.0, 0.0, 5.0, 8.0}};
    std::vector<std::vector<double>> vect2{
        {0.0, 9.0, 1.0, 0.0}, {9.0, 8.0, 8.0, 0.0}, {3.0, 0.0, 5.0, 5.0}, {0.0, 8.0, 3.0, 8.0}};
    Matrix matrix1(4, 4, vect1);
    Matrix matrix2(4, 4, vect2);
    CHECK(matrix1.transposed() == matrix2);
  }
  SUBCASE("Transposing the identity matrix") {
    Matrix matrix1 = Matrix::identity(4);
    CHECK(matrix1.transposed() == matrix1);
  }
  SUBCASE("Calculating the determinant of a 2x2 matrix") {
    std::vector<std::vector<double>> vect1{
        {1.0, 5.0},
        {-3.0, 2.0},
    };
    Matrix matrix1(2, 2, vect1);
    CHECK(matrix1.determinant() == 17.0);
  }
  SUBCASE("Calculating the determinant of a 3x3 matrix") {
    std::vector<std::vector<double>> vect{{1.0, 2.0, 6.0}, {-5.0, 8.0, -4.0}, {2.0, 6.0, 4.0}};
    Matrix matrix(3, 3, vect);
    CHECK(matrix.determinant() == -196.0);
  }
  SUBCASE("Calculating the determinant of a 4x4 matrix") {
    std::vector<std::vector<double>> vect{{-2.0, -8.0, 3.0, 5.0},
                                          {-3.0, 1.0, 7.0, 3.0},
                                          {1.0, 2.0, -9.0, 6.0},
                                          {-6.0, 7.0, 7.0, -9.0}};
    Matrix matrix(4, 4, vect);
    CHECK(abs(matrix.determinant() - -4071.0) < 0.0001);
  }
  SUBCASE("Testing an invertible matrix for invertibility") {
    std::vector<std::vector<double>> vect{
        {6.0, 4.0, 4.0, 4.0}, {5.0, 5.0, 7.0, 6.0}, {4.0, -9.0, 3.0, -7.0}, {9.0, 1.0, 7.0, -6.0}};
    Matrix matrix(4, 4, vect);
    CHECK(matrix.invertible() == true);
  }
  SUBCASE("Testing an noninvertible matrix for invertibility") {
    std::vector<std::vector<double>> vect{{-4.0, 2.0, -2.0, -3.0},
                                          {9.0, 6.0, 2.0, 6.0},
                                          {0.0, -5.0, 1.0, -5.0},
                                          {0.0, 0.0, 0.0, 0.0}};
    Matrix matrix(4, 4, vect);
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
    Matrix matrix1(4, 4, vect1);
    Matrix matrix2(4, 4, vect2);
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
    Matrix matrix1(4, 4, vect1);
    Matrix matrix2(4, 4, vect2);
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
    Matrix matrix1(4, 4, vect1);
    Matrix matrix2(4, 4, vect2);
    CHECK(matrix1.inverse() == matrix2);
  }
  SUBCASE("Multiplying a product by its inverse") {
    std::vector<std::vector<double>> vect1{{3.0, -9.0, 7.0, 3.0},
                                           {3.0, -8.0, 2.0, -9.0},
                                           {-4.0, 4.0, 4.0, 1.0},
                                           {-6.0, 5.0, -1.0, 1.0}};
    std::vector<std::vector<double>> vect2{
        {8.0, 2.0, 2.0, 2.0}, {3.0, -1.0, 7.0, 0.0}, {7.0, 0.0, 5.0, 4.0}, {6.0, -2.0, 0.0, 5.0}};
    Matrix matrix1(4, 4, vect1);
    Matrix matrix2(4, 4, vect2);
    Matrix product = matrix1 * matrix2;
    CHECK(product * matrix2.inverse() == matrix1);
  }
}
TEST_CASE("Matrix transformations") {
  using namespace raytracerchallenge;
  SUBCASE("Multiplying by a transformation matrix") {
    Matrix transform = Matrix::translation(5.0, -3.0, 2.0);
    Tuple point = Tuple::point(-3.0, 4.0, 5.0);
    CHECK(transform * point == Tuple::point(2.0, 1.0, 7.0));
  }
  SUBCASE("Multiplying by the inverse of a transformation matrix") {
    Matrix transform = Matrix::translation(5.0, -3.0, 2.0);
    Tuple point = Tuple::point(-3.0, 4.0, 5.0);
    CHECK(transform.inverse() * point == Tuple::point(-8.0, 7.0, 3.0));
  }
  SUBCASE("Translation does not affect vectors") {
    Matrix transform = Matrix::translation(5.0, -3.0, 2.0);
    Tuple vector = Tuple::vector(-3.0, 4.0, 5.0);
    CHECK(transform * vector == vector);
  }
  SUBCASE("A scaling matrix applied to a point") {
    Matrix transform = Matrix::scaling(2.0, 3.0, 4.0);
    Tuple point = Tuple::point(-4.0, 6.0, 8.0);
    CHECK(transform * point == Tuple::point(-8.0, 18.0, 32.0));
  }
  SUBCASE("A scaling matrix applied to a vector") {
    Matrix transform = Matrix::scaling(2.0, 3.0, 4.0);
    Tuple vector = Tuple::vector(-4.0, 6.0, 8.0);
    CHECK(transform * vector == Tuple::vector(-8.0, 18.0, 32.0));
  }
  SUBCASE("Multiplying by the inverse of a scaling matrix") {
    Matrix transform = Matrix::scaling(2.0, 3.0, 4.0);
    Tuple vector = Tuple::vector(-4.0, 6.0, 8.0);
    CHECK(transform.inverse() * vector == Tuple::vector(-2.0, 2.0, 2.0));
  }
  SUBCASE("Reflection is scaling by a negative value") {
    Matrix transform = Matrix::scaling(-1.0, 1.0, 1.0);
    Tuple point = Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == Tuple::point(-2.0, 3.0, 4.0));
  }
  SUBCASE("Rotating a point around the x axis") {
    Tuple point = Tuple::point(0.0, 1.0, 0.0);
    Matrix halfQuarter = Matrix::rotationX(M_PI / 4.0);
    Matrix fullQuarter = Matrix::rotationX(M_PI / 2.0);
    CHECK(halfQuarter * point == Tuple::point(0.0, sqrt(2.0) / 2.0, sqrt(2.0) / 2.0));
    CHECK(fullQuarter * point == Tuple::point(0.0, 0.0, 1.0));
  }
  SUBCASE("The inverse of an x-rotation rotates in the opposite direction") {
    Tuple point = Tuple::point(0.0, 1.0, 0.0);
    Matrix halfQuarter = Matrix::rotationX(M_PI / 4.0);
    CHECK(halfQuarter.inverse() * point == Tuple::point(0.0, sqrt(2.0) / 2.0, -sqrt(2.0) / 2.0));
  }
  SUBCASE("Rotating a point around the y axis") {
    Tuple point = Tuple::point(0.0, 0.0, 1.0);
    Matrix halfQuarter = Matrix::rotationY(M_PI / 4.0);
    Matrix fullQuarter = Matrix::rotationY(M_PI / 2.0);
    CHECK(halfQuarter * point == Tuple::point(sqrt(2.0) / 2.0, 0.0, sqrt(2.0) / 2.0));
    CHECK(fullQuarter * point == Tuple::point(1.0, 0.0, 0.0));
  }
  SUBCASE("Rotating a point around the z axis") {
    Tuple point = Tuple::point(0.0, 1.0, 0.0);
    Matrix halfQuarter = Matrix::rotationZ(M_PI / 4.0);
    Matrix fullQuarter = Matrix::rotationZ(M_PI / 2.0);
    CHECK(halfQuarter * point == Tuple::point(-sqrt(2.0) / 2.0, sqrt(2.0) / 2.0, 0.0));
    CHECK(fullQuarter * point == Tuple::point(-1.0, 0.0, 0.0));
  }
  SUBCASE("A shearing transformation moves x in proportion to y") {
    Matrix transform = Matrix::shearing(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    Tuple point = Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == Tuple::point(5.0, 3.0, 4.0));
  }
  SUBCASE("A shearing transformation moves x in proportion to z") {
    Matrix transform = Matrix::shearing(0.0, 1.0, 0.0, 0.0, 0.0, 0.0);
    Tuple point = Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == Tuple::point(6.0, 3.0, 4.0));
  }
  SUBCASE("A shearing transformation moves y in proportion to x") {
    Matrix transform = Matrix::shearing(0.0, 0.0, 1.0, 0.0, 0.0, 0.0);
    Tuple point = Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == Tuple::point(2.0, 5.0, 4.0));
  }
  SUBCASE("A shearing transformation moves y in proportion to z") {
    Matrix transform = Matrix::shearing(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    Tuple point = Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == Tuple::point(2.0, 7.0, 4.0));
  }
  SUBCASE("A shearing transformation moves z in proportion to x") {
    Matrix transform = Matrix::shearing(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    Tuple point = Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == Tuple::point(2.0, 3.0, 6.0));
  }
  SUBCASE("A shearing transformation moves z in proportion to y") {
    Matrix transform = Matrix::shearing(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    Tuple point = Tuple::point(2.0, 3.0, 4.0);
    CHECK(transform * point == Tuple::point(2.0, 3.0, 7.0));
  }
  SUBCASE("Individual transformations are applied in sequence") {
    Tuple point = Tuple::point(1.0, 0.0, 1.0);
    Matrix transformA = Matrix::rotationX(M_PI / 2.0);
    Matrix transformB = Matrix::scaling(5.0, 5.0, 5.0);
    Matrix transformC = Matrix::translation(10.0, 5.0, 7.0);
    Tuple point2 = transformA * point;
    CHECK(point2 == Tuple::point(1.0, -1.0, 0.0));
    Tuple point3 = transformB * point2;
    CHECK(point3 == Tuple::point(5.0, -5.0, 0.0));
    Tuple point4 = transformC * point3;
    CHECK(point4 == Tuple::point(15.0, 0.0, 7.0));
  }
  SUBCASE("Chained transformations must be applied in reverse order") {
    Tuple point = Tuple::point(1.0, 0.0, 1.0);
    Matrix transformA = Matrix::rotationX(M_PI / 2.0);
    Matrix transformB = Matrix::scaling(5.0, 5.0, 5.0);
    Matrix transformC = Matrix::translation(10.0, 5.0, 7.0);
    Matrix transforms = transformC * transformB * transformA;
    CHECK(transforms * point == Tuple::point(15.0, 0.0, 7.0));
  }
  SUBCASE("Fluent transformations") {
    Tuple point = Tuple::point(1.0, 0.0, 1.0);
    Matrix transform = Matrix::identity(4)
                           .rotatedX(M_PI / 2.0)
                           .rotatedY(M_PI / 2.0)
                           .rotatedZ(M_PI / 2.0)
                           .scaled(5.0, 5.0, 5.0)
                           .translated(10.0, 5.0, 7.0)
                           .sheared(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    CHECK(transform * point == Tuple::point(20.0, 5.0, 2.0));
  }
  SUBCASE("The transformation matrix for the default orientation") {
    auto from = Tuple::point(0.0, 0.0, 0.0);
    auto to = Tuple::point(0.0, 0.0, -1.0);
    auto up = Tuple::vector(0.0, 1.0, 0.0);
    auto transformation = Matrix::view(from, to, up);
    CHECK(transformation == Matrix::identity(4));
  }
  SUBCASE("A view transformation matrix looking in positive z direction") {
    auto from = Tuple::point(0.0, 0.0, 0.0);
    auto to = Tuple::point(0.0, 0.0, 1.0);
    auto up = Tuple::vector(0.0, 1.0, 0.0);
    auto transformation = Matrix::view(from, to, up);
    CHECK(transformation == Matrix::scaling(-1.0, 1.0, -1.0));
  }
  SUBCASE("The view transformation moves the world") {
    auto from = Tuple::point(0.0, 0.0, 8.0);
    auto to = Tuple::point(0.0, 0.0, 1.0);
    auto up = Tuple::vector(0.0, 1.0, 0.0);
    auto transformation = Matrix::view(from, to, up);
    CHECK(transformation == Matrix::translation(0.0, 0.0, -8.0));
  }
  SUBCASE("An arbitrary view transformation") {
    auto from = Tuple::point(1.0, 3.0, 2.0);
    auto to = Tuple::point(4.0, -2.0, 8.0);
    auto up = Tuple::vector(1.0, 1.0, 0.0);
    auto transformation = Matrix::view(from, to, up);
    Matrix matrix(4, 4,
                  {{-0.50709, 0.50709, 0.67612, -2.36643},
                   {0.76772, 0.60609, 0.12122, -2.82843},
                   {-0.35857, 0.59761, -0.71714, 0.00000},
                   {0.00000, 0.00000, 0.00000, 1.00000}});
    CHECK(transformation == matrix);
  }
}