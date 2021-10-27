#pragma once

#include <raytracerchallenge/shapes/Shape.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a flat surface
   */
  class Plane : public Shape {
  public:
    Plane() = default;
    static std::shared_ptr<Shape> create() {
      auto shape = new Plane();
      return shape->sharedPtr;
    }
    Tuple localNormalAt(Tuple point, Intersection hit) override;
    Intersections localIntersect(Ray ray) override;
    BoundingBox bounds() override;
  };
}  // namespace raytracerchallenge