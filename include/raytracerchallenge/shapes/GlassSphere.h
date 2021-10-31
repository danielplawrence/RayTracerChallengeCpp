#pragma once

#include <raytracerchallenge/shapes/Sphere.h>

namespace raytracerchallenge {
  /**
   * @brief A glass sphere
   */
  class GlassSphere : public Sphere {
  public:
    GlassSphere() {
      this->material->transparency = 1.0;
      this->material->refractiveIndex = 1.5;
    }
    static std::shared_ptr<Shape> create() {
      auto shape = new GlassSphere();
      return shape->sharedPtr;
    }
  };
}  // namespace raytracerchallenge