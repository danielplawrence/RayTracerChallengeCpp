#pragma once

#include <raytracerchallenge/base/Tuple.h>
#include <raytracerchallenge/shapes/Shape.h>

#include <memory>

namespace raytracerchallenge {
  /**
   * Ray intersection computations
   */
  class Computations {
  public:
    double t{};
    std::shared_ptr<Shape> object;
    Tuple point;
    Tuple overPoint;
    Tuple underPoint;
    Tuple eyeVector;
    Tuple normalVector;
    Tuple reflectionVector;
    double n1;
    double n2;
    bool inside{};
    /**
     * Calculate the Schlick approximation for these computations
     * @param computations
     * @return schlick approximation
     */
    static double schlick(const Computations &computations);
  };
}  // namespace raytracerchallenge