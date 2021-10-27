#pragma once

#include <raytracerchallenge/shapes/Shape.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a cylinder
   */
  class Cylinder : public Shape {
  public:
    Cylinder() = default;
    static std::shared_ptr<Shape> create(double min = double(-INFINITY), double max = INFINITY,
                                         bool closed = false) {
      auto shape = new Cylinder();
      shape->maximum = max;
      shape->minimum = min;
      shape->closed = closed;
      return shape->sharedPtr;
    }
    Tuple localNormalAt(Tuple point, Intersection hit) override;
    Intersections localIntersect(Ray ray) override;
    BoundingBox bounds() override;
    double minimum = double(-INFINITY);
    double maximum = double(INFINITY);
    bool closed = false;
  };
}  // namespace raytracerchallenge