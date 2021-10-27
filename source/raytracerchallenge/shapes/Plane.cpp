
#include <raytracerchallenge/Constants.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Plane.h>

namespace raytracerchallenge {
  Tuple Plane::localNormalAt(Tuple point, Intersection hit) {
    (void)point;
    (void)hit;
    return Tuple::vector(0.0, 1.0, 0.0);
  }
  Intersections Plane::localIntersect(Ray ray) {
    if (abs(ray.direction.y) < EPS) {
      return {};
    }
    auto t = -ray.origin.y / ray.direction.y;
    return Intersections({Intersection(t, this->sharedPtr)});
  }
  BoundingBox Plane::bounds() {
    return {Tuple(NEGATIVE_INFINITY, 0.0, NEGATIVE_INFINITY, 1.0),
            Tuple(INFINITY, 0.0, INFINITY, 1.0)};
  }
}  // namespace raytracerchallenge