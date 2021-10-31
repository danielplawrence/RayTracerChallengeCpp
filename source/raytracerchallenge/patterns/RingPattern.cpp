#include <raytracerchallenge/patterns/RingPattern.h>

namespace raytracerchallenge {
  RingPattern::RingPattern(Color a, Color b) {
    this->a = a;
    this->b = b;
  }
  Color RingPattern::colorAt(std::shared_ptr<Shape> shape, Tuple point) {
    auto objectPoint = shape->transform.inverse() * point;
    auto patternPoint = this->transform.inverse() * objectPoint;
    auto xSquared = pow(patternPoint.x, 2);
    auto zSquared = pow(patternPoint.z, 2);
    if (int(floor(sqrt(xSquared + zSquared))) % 2 == 0) {
      return this->a;
    }
    return this->b;
  }
}  // namespace raytracerchallenge