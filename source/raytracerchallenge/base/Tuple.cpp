
#include <raytracerchallenge/Constants.h>
#include <raytracerchallenge/base/Tuple.h>

#include <cmath>

namespace raytracerchallenge {
  bool doubleEquals(double x, double y) {
    if (x == INFINITY && y == INFINITY) {
      return true;
    }
    if (x == -1.0 * INFINITY && y == -1.0 * INFINITY) {
      return true;
    }
    return abs(x - y) < EPS;
  }
  Tuple::Tuple() {
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->w = 0.0;
  }
  Tuple::Tuple(double x, double y, double z, double w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  bool Tuple::isVector() const { return w == 0; }
  double Tuple::magnitude() const {
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2) + pow(this->w, 2));
  }
  Tuple Tuple::normalize() const {
    return {this->x / this->magnitude(), this->y / this->magnitude(), this->z / this->magnitude(),
            this->w / this->magnitude()};
  }
  bool Tuple::operator==(const Tuple &t) const {
    return doubleEquals(t.x, this->x) && doubleEquals(t.y, this->y) && doubleEquals(t.z, this->z)
           && doubleEquals(t.w, this->w);
  }
  double Tuple::dot(const Tuple &t) const {
    return this->x * t.x + this->y * t.y + this->z * t.z + this->w * t.w;
  }
  Tuple Tuple::cross(const Tuple &t) const {
    return Tuple::vector(this->y * t.z - this->z * t.y, this->z * t.x - this->x * t.z,
                         this->x * t.y - this->y * t.x);
  }
  Tuple Tuple::reflect(const Tuple &t) const { return *this - t * 2.0 * this->dot(t); }
  Tuple Tuple::operator+(const Tuple &t) const {
    return {t.x + this->x, t.y + this->y, t.z + this->z, t.w + this->w};
  }
  Tuple Tuple::operator-(const Tuple &t) const {
    return {this->x - t.x, this->y - t.y, this->z - t.z, this->w - t.w};
  }
  Tuple Tuple::operator-() const { return {-this->x, -this->y, -this->z, -this->w}; }
  Tuple Tuple::operator*(double f) const {
    return {f * this->x, f * this->y, f * this->z, f * this->w};
  }
  Tuple Tuple::operator/(double f) const {
    return {this->x / f, this->y / f, this->z / f, this->w / f};
  }
  Tuple Tuple::point(double x, double y, double z) { return {x, y, z, 1.0}; }
  Tuple Tuple::vector(double x, double y, double z) { return {x, y, z, 0.0}; }
}  // namespace raytracerchallenge