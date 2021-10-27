#pragma once

#include <raytracerchallenge/patterns/Pattern.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a checkers pattern
   */
  class CheckersPattern : public Pattern {
  private:
    Color a;
    Color b;

  public:
    CheckersPattern(Color a, Color b);
    [[nodiscard]] Color colorAt(std::shared_ptr<Shape> shape, Tuple point) const override;
  };
}  // namespace raytracerchallenge