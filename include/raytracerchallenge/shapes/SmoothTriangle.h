#pragma once

#include <raytracerchallenge/shapes/Triangle.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a smooth triangle
   */
  class SmoothTriangle : public Triangle {
  public:
    Tuple n1;
    Tuple n2;
    Tuple n3;
    SmoothTriangle(Tuple p1, Tuple p2, Tuple p3, Tuple n1, Tuple n2, Tuple n3) {
      this->p1 = p1;
      this->p2 = p2;
      this->p3 = p3;
      this->n1 = n1;
      this->n2 = n2;
      this->n3 = n3;
      this->e1 = p2 - p1;
      this->e2 = p3 - p1;
    }
    static std::shared_ptr<Shape> create(Tuple p1, Tuple p2, Tuple p3, Tuple n1, Tuple n2,
                                         Tuple n3) {
      return (new SmoothTriangle(p1, p2, p3, n1, n2, n3))->sharedPtr;
    }
    Tuple localNormalAt(Tuple point, Intersection hit) override;
    BoundingBox bounds() override;
  };
}  // namespace raytracerchallenge