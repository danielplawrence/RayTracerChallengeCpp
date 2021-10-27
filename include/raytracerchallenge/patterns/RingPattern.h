#pragma once

#include <raytracerchallenge/patterns/Pattern.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a ring pattern
   */
  class RingPattern : public Pattern {
  private:
    Color a;
    Color b;

  public:
    RingPattern(Color a, Color b);
    [[nodiscard]] Color colorAt(std::shared_ptr<Shape> shape, Tuple point) const override;
  };
}  // namespace raytracerchallenge