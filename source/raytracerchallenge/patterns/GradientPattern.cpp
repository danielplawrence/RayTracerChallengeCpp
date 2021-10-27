#include <raytracerchallenge/patterns/GradientPattern.h>

namespace raytracerchallenge {
  GradientPattern::GradientPattern(Color a, Color b) {
    this->a = a;
    this->b = b;
  }
  Color GradientPattern::colorAt(std::shared_ptr<Shape> shape, Tuple point) const {
    auto objectPoint = shape->transform.inverse() * point;
    auto patternPoint = this->transform.inverse() * objectPoint;
    auto distance = this->b - this->a;
    auto fraction = patternPoint.x - floor(patternPoint.x);
    return this->a + distance * fraction;
  }
}  // namespace raytracerchallenge