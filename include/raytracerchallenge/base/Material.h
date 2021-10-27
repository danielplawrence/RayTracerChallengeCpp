#pragma once

#include <raytracerchallenge/base/Color.h>
#include <raytracerchallenge/patterns/Pattern.h>

namespace raytracerchallenge {
  class Pattern;
  /**
   * @brief Represents a material
   */
  class Material {
  public:
    Pattern *pattern = nullptr;
    double ambient = 0.1;
    double diffuse = 0.9;
    double specular = 0.9;
    double shininess = 200.0;
    double reflective = 0.0;
    double transparency = 0.0;
    double refractiveIndex = 1.0;
    bool castShadow = true;
    Color color = Color(1.0, 1.0, 1.0);
    bool operator==(const Material &material) const {
      return this->ambient == material.ambient && this->diffuse == material.diffuse
             && this->specular == material.specular && this->shininess == material.shininess;
    }
  };
}  // namespace raytracerchallenge