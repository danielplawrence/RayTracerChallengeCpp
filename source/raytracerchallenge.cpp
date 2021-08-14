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
bool RayTracerChallenge::Tuple::operator==(const RayTracerChallenge::Tuple &t) const {
  return floatEquals(t.x, this->x) && floatEquals(t.y, this->y) && floatEquals(t.z, this->z)
         && floatEquals(t.w, this->w);
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::point(float x, float y, float z) {
  return {x, y, z, 1.0};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::vector(float x, float y, float z) {
  return {x, y, z, 0.0};
}
bool RayTracerChallenge::Tuple::floatEquals(float x, float y) { return abs(x - y) < 0.00001; }
