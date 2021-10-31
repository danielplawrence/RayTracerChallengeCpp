
#include <raytracerchallenge/Constants.h>
#include <raytracerchallenge/base/Computations.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Shape.h>

namespace raytracerchallenge {
  Intersection::Intersection(double t, std::shared_ptr<Shape> object) {
    this->t = t;
    this->object = std::move(object);
  }
  Intersection::Intersection() = default;
  bool Intersection::operator==(const Intersection &intersection) const {
    return this->object->is(*intersection.object) && this->t == intersection.t;
  }
  bool Intersection::operator<(const Intersection &intersection) const {
    return this->t < intersection.t;
  }
  Computations Intersection::prepareComputations(Ray ray) const {
    auto computations = Computations();
    computations.t = this->t;
    computations.object = this->object;
    computations.point = ray.position(t);
    computations.eyeVector = -ray.direction;
    computations.normalVector = this->object->normalAt(computations.point, *this);
    if (computations.normalVector.dot(computations.eyeVector) < 0.0) {
      computations.inside = true;
      computations.normalVector = -computations.normalVector;
    }
    computations.reflectionVector = ray.direction.reflect(computations.normalVector);
    computations.overPoint = computations.point + (computations.normalVector * EPS);
    computations.underPoint = computations.point - (computations.normalVector * EPS);
    return computations;
  }
  Computations Intersection::prepareComputations(Ray ray,
                                                 const Intersections &intersections) const {
    auto computations = prepareComputations(ray);
    std::vector<std::shared_ptr<Shape>> containers;
    for (const Intersection &i : intersections.intersections) {
      if (i == *this) {
        if (containers.empty()) {
          computations.n1 = 1.0;
        } else {
          computations.n1 = containers.back()->material->refractiveIndex;
        }
      }
      auto position = std::find(containers.begin(), containers.end(), i.object);
      if (position != containers.end()) {
        containers.erase(position);
      } else {
        containers.push_back(i.object);
      }
      if (i == *this) {
        if (containers.empty()) {
          computations.n2 = 1.0;
        } else {
          computations.n2 = containers.back()->material->refractiveIndex;
        }
        break;
      }
    }
    return computations;
  }
  std::optional<Intersection> Intersections::hit() const {
    std::vector<Intersection> nonNegative;
    std::copy_if(intersections.cbegin(), intersections.cend(), std::back_inserter(nonNegative),
                 [](const Intersection &i) { return i.t >= 0.0; });
    if (nonNegative.empty()) {
      return {};
    }
    return *std::min_element(nonNegative.begin(), nonNegative.end());
  }
  Intersections::Intersections() = default;
  void Intersections::Intersections::addAll(Intersections newIntersections) {
    this->intersections.insert(this->intersections.end(), newIntersections.intersections.begin(),
                               newIntersections.intersections.end());
  }
  Intersections::Intersections(std::vector<Intersection> intersections) {
    this->intersections = std::move(intersections);
  }
  Intersection Intersections::operator[](unsigned int x) const { return this->intersections[x]; }
  size_t Intersections::size() const { return this->intersections.size(); }
  void Intersections::sort() { std::sort(this->intersections.begin(), this->intersections.end()); }
}  // namespace raytracerchallenge