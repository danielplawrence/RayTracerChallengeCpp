#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/SmoothTriangle.h>

namespace raytracerchallenge {
  Tuple SmoothTriangle::localNormalAt(Tuple point, Intersection hit) {
    (void)point;
    return this->n2 * hit.u + this->n3 * hit.v + this->n1 * (1 - hit.u - hit.v);
  }
  BoundingBox SmoothTriangle::bounds() {
    auto b = BoundingBox();
    b.add(this->p1);
    b.add(this->p2);
    b.add(this->p3);
    return b;
  }
}  // namespace raytracerchallenge