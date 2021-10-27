#include <raytracerchallenge/Constants.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Cylinder.h>

namespace raytracerchallenge {
  Tuple Cylinder::localNormalAt(Tuple point, Intersection hit) {
    (void)hit;
    auto dist = pow(point.x, 2.0) + pow(point.z, 2.0);
    if (dist < 1.0 && point.y >= this->maximum - EPS) {
      return {0.0, 1.0, 0.0, 0.0};
    } else if (dist < 1.0 && point.y <= this->minimum + EPS) {
      return {0.0, -1.0, 0.0, 0.0};
    }
    return {point.x, 0.0, point.z, 0.0};
  }
  bool checkCap(Ray ray, double t, double capRadius = 1.0) {
    auto x = ray.origin.x + t * ray.direction.x;
    auto z = ray.origin.z + t * ray.direction.z;
    return (pow(x, 2) + pow(z, 2)) <= capRadius;
  }
  void intersectCaps(Cylinder &cyl, Ray ray, Intersections &xs) {
    if (!cyl.closed || abs(ray.direction.y) < EPS) {
      return;
    }
    auto t = (cyl.minimum - ray.origin.y) / ray.direction.y;
    if (checkCap(ray, t)) {
      xs.intersections.emplace_back(t, cyl.sharedPtr);
    }
    t = (cyl.maximum - ray.origin.y) / ray.direction.y;
    if (checkCap(ray, t)) {
      xs.intersections.emplace_back(t, cyl.sharedPtr);
    }
  }
  Intersections Cylinder::localIntersect(Ray ray) {
    auto a = pow(ray.direction.x, 2) + pow(ray.direction.z, 2);
    auto b = 2 * ray.origin.x * ray.direction.x + 2 * ray.origin.z * ray.direction.z;
    auto c = pow(ray.origin.x, 2) + pow(ray.origin.z, 2) - 1.0;
    auto disc = pow(b, 2) - 4.0 * a * c;
    if (disc < 0) {
      return {};
    }
    auto t0 = (-b - sqrt(disc)) / (2.0 * a);
    auto t1 = (-b + sqrt(disc)) / (2.0 * a);
    if (t0 > t1) {
      std::swap(t0, t1);
    }
    auto xs = Intersections();
    auto y0 = ray.origin.y + t0 * ray.direction.y;
    if (this->minimum < y0 && y0 < this->maximum) {
      xs.intersections.emplace_back(t0, this->sharedPtr);
    }
    auto y1 = ray.origin.y + t1 * ray.direction.y;
    if (this->minimum < y1 && y1 < this->maximum) {
      xs.intersections.emplace_back(t1, this->sharedPtr);
    }
    intersectCaps(*this, ray, xs);
    return xs;
  }
  BoundingBox Cylinder::bounds() {
    if (this->closed) {
      return {{-1.0, this->minimum, -1.0, 1.0}, {1.0, this->maximum, 1.0, 1.0}};
    }
    return {{-1.0, NEGATIVE_INFINITY, -1.0, 1.0}, {1.0, INFINITY, 1.0, 1.0}};
  }
}  // namespace raytracerchallenge