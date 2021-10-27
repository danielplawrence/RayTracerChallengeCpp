#pragma once

#include <raytracerchallenge/base/Color.h>
#include <raytracerchallenge/base/Tuple.h>
#include <raytracerchallenge/shapes/Shape.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a light source
   */
  class PointLight {
  public:
    Color intensity;
    Tuple position;
    /**
     * @brief Construct a new Light
     * @param position Tuple representing the position of the light
     * @param intensity Color of the light
     */
    PointLight(Tuple position, Color intensity) {
      this->position = position;
      this->intensity = intensity;
    }
  };
  /**
   * Calculate the lighting at a particular position on a material
   * using the Phong reflection model
   * @param shape Target object
   * @param light Light source
   * @param position Position we need the lighting for
   * @param eyeVector Eye vector
   * @param normalVector Normal vector
   * @param inShadow Whether or not the position is in shadow
   * @return The color for the target position
   */
  Color lighting(const std::shared_ptr<Shape> &shape, PointLight light, Tuple position,
                 Tuple eyeVector, Tuple normalVector, bool inShadow);
}  // namespace raytracerchallenge