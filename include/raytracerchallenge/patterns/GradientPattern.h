#pragma once

#include <raytracerchallenge/patterns/Pattern.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a gradient
   */
  class GradientPattern : public Pattern {
  private:
    Color a;
    Color b;

  public:
    GradientPattern(Color a, Color b);
    [[nodiscard]] Color colorAt(std::shared_ptr<Shape> shape, Tuple point) const override;
  };
}  // namespace raytracerchallenge