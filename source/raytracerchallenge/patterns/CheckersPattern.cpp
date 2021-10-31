#include <raytracerchallenge/patterns/CheckersPattern.h>

namespace raytracerchallenge {
  CheckersPattern::CheckersPattern(Color a, Color b) {
    this->a = a;
    this->b = b;
  }
  Color CheckersPattern::colorAt(std::shared_ptr<Shape> shape, Tuple point) {
    auto objectPoint = shape->transform.inverse() * point;
    auto patternPoint = this->transform.inverse() * objectPoint;
    auto vectorSum = int(floor(patternPoint.x) + floor(patternPoint.y) + floor(patternPoint.z));
    if (vectorSum % 2 == 0) {
      return this->a;
    }
    return this->b;
  }
}  // namespace raytracerchallenge