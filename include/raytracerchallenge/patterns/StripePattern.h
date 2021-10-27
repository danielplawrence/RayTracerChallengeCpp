#pragma once

#include <raytracerchallenge/patterns/Pattern.h>

namespace raytracerchallenge {
  /**
   * @brief Represents alternating stripes
   */
  class StripePattern : public Pattern {
  private:
    Color a;
    Color b;

  public:
    StripePattern(Color a, Color b);
    [[nodiscard]] Color colorAt(std::shared_ptr<Shape> shape, Tuple point) const override;
  };
}  // namespace raytracerchallenge