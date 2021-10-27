#include <raytracerchallenge/Constants.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Cone.h>

namespace raytracerchallenge {
  bool checkConeCap(Ray ray, double t, double capRadius = 1.0) {
    auto x = ray.origin.x + t * ray.direction.x;
    auto z = ray.origin.z + t * ray.direction.z;
    return (pow(x, 2) + pow(z, 2)) <= capRadius;
  }
  Tuple Cone::localNormalAt(Tuple point, Intersection hit) {
    (void)hit;
    auto dist = pow(point.x, 2) + pow(point.x, 2);
    if (dist < 1.0 && point.y >= this->maximum - EPS) {
      return {0.0, 1.0, 0.0, 0.0};
    } else if (dist < 1.0 && point.y <= this->minimum + EPS) {
      return {0.0, -1.0, 0.0, 0.0};
    }
    auto y = sqrt(pow(point.x, 2.0) + pow(point.z, 2.0));
    if (point.y > 0.0) {
      y = -y;
    }
    return {point.x, y, point.z, 0.0};
  }
  Intersections Cone::localIntersect(Ray ray) {
    auto xs = Intersections();
    auto a = pow(ray.direction.x, 2) - pow(ray.direction.y, 2) + pow(ray.direction.z, 2);
    auto b = 2 * (ray.origin.x * ray.direction.x) - 2 * (ray.origin.y * ray.direction.y)
             + 2 * (ray.origin.z * ray.direction.z);
    auto c = pow(ray.origin.x, 2) - pow(ray.origin.y, 2) + pow(ray.origin.z, 2);
    if (abs(a) < EPS && abs(b) > EPS) {
      auto t = -c / (2 * b);
      xs.intersections.emplace_back(Intersection(t, this->sharedPtr));
    }
    if (abs(a) > EPS) {
      auto disc = pow(b, 2) - 4.0 * a * c;
      if (disc < 0) {
        return {};
      }
      auto t0 = (-b - sqrt(disc)) / (2.0 * a);
      auto t1 = (-b + sqrt(disc)) / (2.0 * a);
      if (t0 > t1) {
        std::swap(t1, t0);
      }
      auto y0 = ray.origin.y + t0 * ray.direction.y;
      if (this->minimum < y0 && y0 < this->maximum) {
        xs.intersections.emplace_back(t0, this->sharedPtr);
      }
      auto y1 = ray.origin.y + t1 * ray.direction.y;
      if (this->minimum < y1 && y1 < this->maximum) {
        xs.intersections.emplace_back(t1, this->sharedPtr);
      }
    }
    if (!this->closed || abs(ray.direction.y - 0) < EPS) {
      return xs;
    }
    auto t = (this->minimum - ray.origin.y) / ray.direction.y;
    if (checkConeCap(ray, t, abs(minimum))) {
      xs.intersections.emplace_back(t, this->sharedPtr);
    }
    t = (this->maximum - ray.origin.y) / ray.direction.y;
    if (checkConeCap(ray, t, abs(maximum))) {
      xs.intersections.emplace_back(t, this->sharedPtr);
    }
    return xs;
  }
  BoundingBox Cone::bounds() {
    if (closed) {
      auto a = abs(this->minimum);
      auto b = abs(this->maximum);
      auto limit = fmax(a, b);
      return {
          {-limit, this->minimum, -limit, 1.0},
          {limit, this->maximum, limit, 1.0},
      };
    }
    return {{NEGATIVE_INFINITY, NEGATIVE_INFINITY, NEGATIVE_INFINITY, 1.0},
            {INFINITY, INFINITY, INFINITY, 1.0}};
  }
}  // namespace raytracerchallenge