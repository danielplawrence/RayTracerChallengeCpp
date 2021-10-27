#pragma once

#include <raytracerchallenge/base/Matrix.h>
#include <raytracerchallenge/base/Tuple.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a ray of light
   */
  class Ray {
  public:
    /**
     * @brief Starting point of ray
     */
    Tuple origin;
    /**
     * @brief Vector representing the ray's direction
     */
    Tuple direction;
    /**
     * @brief Construct a ray of light
     * @param origin origin of the ray
     * @param direction direction of the ray
     */
    Ray(Tuple origin, Tuple direction);
    /**
     * @brief Return the position at point t along the ray
     */
    [[nodiscard]] Tuple position(double t) const;
    /**
     * @brief Return the transformation of this ray using the provided transformation matrix
     * @param matrix transformation matrix
     * @return a new Ray with the transformation applied
     */
    [[nodiscard]] Ray transform(const Matrix &matrix) const;
  };
}  // namespace raytracerchallenge