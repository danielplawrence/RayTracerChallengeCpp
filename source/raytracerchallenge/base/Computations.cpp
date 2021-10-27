
#include <raytracerchallenge/base/Computations.h>

namespace raytracerchallenge {
  double Computations::schlick(const Computations &computations) {
    auto cos = computations.eyeVector.dot(computations.normalVector);
    if (computations.n1 > computations.n2) {
      auto n = computations.n1 / computations.n2;
      auto sin2T = pow(n, 2.0) * (1.0 - pow(cos, 2.0));
      if (sin2T > 1.0) {
        return 1.0;
      }
      auto cosT = sqrt(1.0 - sin2T);
      cos = cosT;
    }
    auto r0 = pow(((computations.n1 - computations.n2) / (computations.n1 + computations.n2)), 2.0);
    return r0 + (1 - r0) * pow((1 - cos), 5.0);
  }
}  // namespace raytracerchallenge