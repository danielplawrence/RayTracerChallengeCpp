#pragma once

#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Shape.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a sphere
   */
  class Sphere : public Shape {
  public:
    Sphere() = default;
    static std::shared_ptr<Shape> create() {
      auto shape = new Sphere();
      return shape->sharedPtr;
    }
    Tuple localNormalAt(Tuple point, Intersection hit) override;
    Intersections localIntersect(Ray ray) override;
    BoundingBox bounds() override;
  };
}  // namespace raytracerchallenge