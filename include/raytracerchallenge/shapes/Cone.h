#pragma once

#include <raytracerchallenge/shapes/Cylinder.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a cone
   */
  class Cone : public Cylinder {
  public:
    static std::shared_ptr<Shape> create(double min = double(-INFINITY), double max = INFINITY,
                                         bool closed = false) {
      auto shape = new Cone();
      shape->maximum = max;
      shape->minimum = min;
      shape->closed = closed;
      return shape->sharedPtr;
    }
    Tuple localNormalAt(Tuple point, Intersection hit) override;
    Intersections localIntersect(Ray ray) override;
    BoundingBox bounds() override;
  };
}  // namespace raytracerchallenge