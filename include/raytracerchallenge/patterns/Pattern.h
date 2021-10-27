#pragma once

#include <raytracerchallenge/shapes/Shape.h>

namespace raytracerchallenge {
  class Shape;
  /**
   * @brief Represents a pattern
   */
  class Pattern {
  public:
    Matrix transform = Matrix::identity(4);
    /**
     * Get the color at this point on this shape
     * @param shape
     * @param point
     * @return Color
     */
    [[nodiscard]] virtual Color colorAt(std::shared_ptr<Shape> shape, Tuple point) const = 0;
  };
}  // namespace raytracerchallenge