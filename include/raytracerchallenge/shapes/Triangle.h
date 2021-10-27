#pragma once

#include <raytracerchallenge/shapes/Shape.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a triangle
   */
  class Triangle : public Shape {
  public:
    Tuple p1;
    Tuple p2;
    Tuple p3;
    Tuple e1;
    Tuple e2;
    Tuple normal;
    Triangle() = default;
    Triangle(Tuple p1, Tuple p2, Tuple p3) {
      this->p1 = p1;
      this->p2 = p2;
      this->p3 = p3;
      this->e1 = p2 - p1;
      this->e2 = p3 - p1;
      this->normal = e2.cross(e1).normalize();
    }
    static std::shared_ptr<Shape> create(Tuple p1, Tuple p2, Tuple p3) {
      return (new Triangle(p1, p2, p3))->sharedPtr;
    }
    Tuple localNormalAt(Tuple point, Intersection hit) override;
    Intersections localIntersect(Ray ray) override;
    BoundingBox bounds() override;
  };
}  // namespace raytracerchallenge