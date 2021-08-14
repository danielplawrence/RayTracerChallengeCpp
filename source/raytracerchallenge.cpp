#include <fmt/format.h>
#include <raytracerchallenge/raytracerchallenge.h>

using namespace raytracerchallenge;

RayTracerChallenge::Tuple::Tuple(float x, float y, float z, float w) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
};
bool RayTracerChallenge::Tuple::isVector() const { return w == 0; }
float RayTracerChallenge::Tuple::magnitude() const {
  return sqrtf(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2) + pow(this->w, 2));
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::normalize() const {
  return {this->x / this->magnitude(), this->y / this->magnitude(), this->z / this->magnitude(),
          this->w / this->magnitude()};
}
bool RayTracerChallenge::Tuple::operator==(const RayTracerChallenge::Tuple &t) const {
  return floatEquals(t.x, this->x) && floatEquals(t.y, this->y) && floatEquals(t.z, this->z)
         && floatEquals(t.w, this->w);
}
float RayTracerChallenge::Tuple::dot(const RayTracerChallenge::Tuple &t) const {
  return this->x * t.x + this->y * t.y + this->z * t.z + this->w * t.w;
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::cross(
    const RayTracerChallenge::Tuple &t) const {
  return RayTracerChallenge::Tuple::vector(
      this->y * t.z - this->z * t.y, this->z * t.x - this->x * t.z, this->x * t.y - this->y * t.x);
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator+(const RayTracerChallenge::Tuple &t) {
  return {t.x + this->x, t.y + this->y, t.z + this->z, t.w + this->w};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator-(const RayTracerChallenge::Tuple &t) {
  return {this->x - t.x, this->y - t.y, this->z - t.z, this->w - t.w};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator-() {
  return {-this->x, -this->y, -this->z, -this->w};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator*(float f) {
  return {f * this->x, f * this->y, f * this->z, f * this->w};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator/(float f) {
  return {this->x / f, this->y / f, this->z / f, this->w / f};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::point(float x, float y, float z) {
  return {x, y, z, 1.0};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::vector(float x, float y, float z) {
  return {x, y, z, 0.0};
}
bool RayTracerChallenge::Tuple::floatEquals(float x, float y) { return abs(x - y) < 0.00001; }
