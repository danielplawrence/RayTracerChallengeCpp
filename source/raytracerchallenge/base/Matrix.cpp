//
// Created by Lawrence, Daniel on 10/24/21.
//
#include <raytracerchallenge/Constants.h>
#include <raytracerchallenge/base/Matrix.h>

#include <cmath>
#include <optional>
#include <vector>

namespace raytracerchallenge {
  Matrix::Matrix(unsigned int x, unsigned int y, std::vector<std::vector<double>> m) {
    MatrixXd mat(x, y);
    for (auto i = 0U; i < x; i++) {
      mat.row(i) = VectorXd::Map(&m[i][0], (long)m[i].size());
    }
    this->m = mat;
  }
  Matrix::Matrix() = default;
  Matrix::Matrix(Eigen::MatrixXd m) { this->m = std::move(m); }
  bool Matrix::operator==(const Matrix &matrix) const { return this->m.isApprox(matrix.m, EPS); }
  bool Matrix::operator!=(const Matrix &matrix) const { return this->m != matrix.m; }
  Matrix Matrix::operator*(const Matrix &matrix) const { return Matrix(this->m * matrix.m); }
  Tuple Matrix::operator*(const Tuple &tuple) const {
    Tuple res = Tuple(0, 0, 0, 0);
    res.x = Tuple(this->m(0, 0), this->m(0, 1), this->m(0, 2), this->m(0, 3)).dot(tuple);
    res.y = Tuple(this->m(1, 0), this->m(1, 1), this->m(1, 2), this->m(1, 3)).dot(tuple);
    res.z = Tuple(this->m(2, 0), this->m(2, 1), this->m(2, 2), this->m(2, 3)).dot(tuple);
    res.w = Tuple(this->m(3, 0), this->m(3, 1), this->m(3, 2), this->m(3, 3)).dot(tuple);
    return res;
  }
  Matrix Matrix::identity(unsigned int size) { return Matrix(MatrixXd::Identity(size, size)); }
  Matrix Matrix::transposed() { return Matrix(this->m.transpose()); }

  double Matrix::determinant() const { return this->m.determinant(); }
  bool Matrix::invertible() const { return determinant() != 0.0; }
  Matrix Matrix::inverse() {
    if (this->inv != nullptr) {
      return *this->inv;
    }
    std::lock_guard<std::mutex> lock(this->mx);
    this->inv = std::shared_ptr<Matrix>(new Matrix(this->m.inverse()));
    return *this->inv;
  }
  Matrix Matrix::translation(double x, double y, double z) {
    Matrix res = Matrix::identity(4);
    res.m(0, 3) = x;
    res.m(1, 3) = y;
    res.m(2, 3) = z;
    return res;
  }
  Matrix Matrix::scaling(double x, double y, double z) {
    Matrix res = Matrix::identity(4);
    res.m(0, 0) = x;
    res.m(1, 1) = y;
    res.m(2, 2) = z;
    res.m(3, 3) = 1;
    return res;
  }
  Matrix Matrix::rotationX(double radians) {
    Matrix res = Matrix::identity(4);
    res.m(1, 1) = cos(radians);
    res.m(1, 2) = -sin(radians);
    res.m(2, 1) = sin(radians);
    res.m(2, 2) = cos(radians);
    return res;
  }
  Matrix Matrix::rotationY(double radians) {
    Matrix res = Matrix::identity(4);
    res.m(0, 0) = cos(radians);
    res.m(0, 2) = sin(radians);
    res.m(2, 0) = -sin(radians);
    res.m(2, 2) = cos(radians);
    return res;
  }
  Matrix Matrix::rotationZ(double radians) {
    Matrix res = Matrix::identity(4);
    res.m(0, 0) = cos(radians);
    res.m(0, 1) = -sin(radians);
    res.m(1, 0) = sin(radians);
    res.m(1, 1) = cos(radians);
    return res;
  }
  Matrix Matrix::shearing(double xy, double xz, double yx, double yz, double zx, double zy) {
    Matrix res = Matrix::identity(4);
    res.m(0, 1) = xy;
    res.m(0, 2) = xz;
    res.m(1, 0) = yx;
    res.m(1, 2) = yz;
    res.m(2, 0) = zx;
    res.m(2, 1) = zy;
    return res;
  }
  Matrix Matrix::translated(double x, double y, double z) const {
    Matrix t = translation(x, y, z);
    return t * *this;
  }
  Matrix Matrix::scaled(double x, double y, double z) const {
    Matrix t = scaling(x, y, z);
    return t * *this;
  }
  Matrix Matrix::rotatedX(double radians) const {
    Matrix t = rotationX(radians);
    return t * *this;
  }
  Matrix Matrix::rotatedY(double radians) const {
    Matrix t = rotationY(radians);
    return t * *this;
  }
  Matrix Matrix::rotatedZ(double radians) const {
    Matrix t = rotationZ(radians);
    return t * *this;
  }
  Matrix Matrix::sheared(double xy, double xz, double yx, double yz, double zx, double zy) const {
    Matrix t = shearing(xy, xz, yx, yz, zx, zy);
    return t * *this;
  }
  Matrix Matrix::view(Tuple from, Tuple to, Tuple up) {
    auto forward = (to - from).normalize();
    auto upN = up.normalize();
    auto left = forward.cross(upN);
    auto trueUp = left.cross(forward);
    Matrix orientation(4, 4,
                       {{left.x, left.y, left.z, 0.0},
                        {trueUp.x, trueUp.y, trueUp.z, 0.0},
                        {-forward.x, -forward.y, -forward.z, 0.0},
                        {0.0, 0.0, 0.0, 1.0}});
    return orientation * translation(-from.x, -from.y, -from.z);
  }
  Matrix::Matrix(Matrix &m) { this->m = m.m; }
  Matrix &Matrix::operator=(const Matrix &mat) {
    if (this == &mat) return *this;
    this->m = mat.m;
    return *this;
  }
}  // namespace raytracerchallenge