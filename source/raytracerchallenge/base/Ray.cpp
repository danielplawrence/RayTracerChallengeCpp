
#include <raytracerchallenge/base/Matrix.h>
#include <raytracerchallenge/base/Ray.h>
#include <raytracerchallenge/base/Tuple.h>

namespace raytracerchallenge {
  Ray::Ray(Tuple origin, Tuple direction) {
    this->origin = origin;
    this->direction = direction;
  }
  Tuple Ray::position(double t) const { return Tuple(this->origin + this->direction * t); }
  Ray Ray::transform(const Matrix &matrix) const {
    return {matrix * this->origin, matrix * this->direction};
  }
}  // namespace raytracerchallenge