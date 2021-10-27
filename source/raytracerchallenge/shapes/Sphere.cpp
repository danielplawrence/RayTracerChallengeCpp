
#include <raytracerchallenge/base/BoundingBox.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/base/Ray.h>
#include <raytracerchallenge/shapes/Sphere.h>

namespace raytracerchallenge {
  Tuple Sphere::localNormalAt(Tuple point, Intersection hit) {
    (void)hit;
    return point - Tuple::point(0.0, 0.0, 0.0);
  }
  Intersections Sphere::localIntersect(Ray ray) {
    Tuple sphereToRay = ray.origin - Tuple::point(0.0, 0.0, 0.0);
    double a = ray.direction.dot(ray.direction);
    double b = 2.0 * ray.direction.dot(sphereToRay);
    double c = sphereToRay.dot(sphereToRay) - 1.0;
    double discriminant = pow(b, 2.0) - 4.0 * a * c;
    if (discriminant < 0.0) {
      return Intersections(std::vector<Intersection>(0));
    }
    double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
    return Intersections(std::vector<Intersection>{Intersection(t1, this->sharedPtr),
                                                   Intersection(t2, this->sharedPtr)});
  }
  BoundingBox Sphere::bounds() { return {{-1.0, -1.0, -1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}}; }
}  // namespace raytracerchallenge
