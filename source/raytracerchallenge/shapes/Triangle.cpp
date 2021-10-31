#include <raytracerchallenge/Constants.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Triangle.h>

namespace raytracerchallenge {
  Tuple Triangle::localNormalAt(Tuple point, Intersection hit) {
    (void)point;
    (void)hit;
    return this->normal;
  }
  Intersections Triangle::localIntersect(Ray ray) {
    auto dirCrossE2 = ray.direction.cross(this->e2);
    auto det = this->e1.dot(dirCrossE2);
    if (abs(det) < EPS) {
      return {};
    }
    auto f = 1.0 / det;
    auto p1ToOrigin = ray.origin - this->p1;
    auto u = f * p1ToOrigin.dot(dirCrossE2);
    if (u <= EPS || u > 1.0) {
      return {};
    }
    auto originCrossE1 = p1ToOrigin.cross(this->e1);
    auto v = f * ray.direction.dot(originCrossE1);
    if (v <= EPS || (u + v) > 1.0) {
      return {};
    }
    auto t = f * this->e2.dot(originCrossE1);
    auto i = Intersection(t, this->sharedPtr);
    i.u = u;
    i.v = v;
    return Intersections({i});
  }
  BoundingBox Triangle::bounds() {
    auto b = BoundingBox();
    b.add(this->p1);
    b.add(this->p2);
    b.add(this->p3);
    return b;
  }
}  // namespace raytracerchallenge