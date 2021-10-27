#pragma once

#include <raytracerchallenge/shapes/Shape.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a cube
   */
  class Cube : public Shape {
  public:
    Cube() = default;
    static std::shared_ptr<Shape> create() {
      auto shape = new Cube();
      return shape->sharedPtr;
    }
    Tuple localNormalAt(Tuple point, Intersection hit) override;
    Intersections localIntersect(Ray ray) override;
    BoundingBox bounds() override;
  };
}  // namespace raytracerchallenge