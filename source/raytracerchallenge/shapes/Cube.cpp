#include <raytracerchallenge/Constants.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Cube.h>

namespace raytracerchallenge {
  std::array<double, 2> checkAxis(double origin, double direction, double min = -1.0,
                                  double max = 1.0) {
    auto tminNumerator = (min - origin);
    auto tmaxNumerator = (max - origin);
    double tmin;
    double tmax;
    if (abs(direction) >= EPS) {
      tmin = tminNumerator / direction;
      tmax = tmaxNumerator / direction;
    } else {
      tmin = tminNumerator * INFINITY;
      tmax = tmaxNumerator * INFINITY;
    }
    if (tmin > tmax) {
      std::swap(tmin, tmax);
    }
    return {tmin, tmax};
  }
  Intersections Cube::localIntersect(Ray ray) {
    auto xMinMax = checkAxis(ray.origin.x, ray.direction.x);
    auto yMinMax = checkAxis(ray.origin.y, ray.direction.y);
    auto zMinMax = checkAxis(ray.origin.z, ray.direction.z);
    auto tMin = std::max({xMinMax[0], yMinMax[0], zMinMax[0]});
    auto tMax = std::min({xMinMax[1], yMinMax[1], zMinMax[1]});
    if (tMin > tMax) {
      return {};
    }
    return Intersections(std::vector<Intersection>{Intersection(tMin, this->sharedPtr),
                                                   Intersection(tMax, this->sharedPtr)});
  }
  Tuple Cube::localNormalAt(Tuple point, Intersection hit) {
    (void)hit;
    auto maxC = std::max({abs(point.x), abs(point.y), abs(point.z)});
    if (maxC == abs(point.x)) {
      return {point.x, 0.0, 0.0, 0.0};
    }
    if (maxC == abs(point.y)) {
      return {0.0, point.y, 0.0, 0.0};
    }
    return {0.0, 0.0, point.z, 0.0};
  }
  BoundingBox Cube::bounds() { return {{-1.0, -1.0, -1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}}; }
}  // namespace raytracerchallenge