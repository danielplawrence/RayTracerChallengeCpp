#include <raytracerchallenge/patterns/StripePattern.h>

namespace raytracerchallenge {
  StripePattern::StripePattern(Color a, Color b) {
    this->a = a;
    this->b = b;
  }
  Color StripePattern::colorAt(std::shared_ptr<Shape> shape, Tuple point) {
    auto objectPoint = shape->worldToObject(point);
    auto patternPoint = this->transform.inverse() * objectPoint;
    if (int(floor(patternPoint.x)) % 2 == 0) {
      return this->a;
    }
    return this->b;
  }
}  // namespace raytracerchallenge