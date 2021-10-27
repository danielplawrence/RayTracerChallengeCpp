#include <raytracerchallenge/base/World.h>
#include <raytracerchallenge/shapes/Sphere.h>

namespace raytracerchallenge {
  World::World() = default;
  bool World::isEmpty() const { return this->objects.empty(); }
  void World::add(const std::shared_ptr<Shape> &object) { this->objects.push_back(object); }
  World World::defaultWorld() {
    World world;
    world.light = PointLight(Tuple::point(-10.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));
    auto sphere1 = Sphere::create();
    sphere1->material.color = Color(0.8, 1.0, 0.6);
    sphere1->material.diffuse = 0.7;
    sphere1->material.specular = 0.2;
    auto sphere2 = Sphere::create();
    sphere2->transform = Matrix::scaling(0.5, 0.5, 0.5);
    world.add(sphere1);
    world.add(sphere2);
    return world;
  }
  Intersections World::intersect(Ray ray) {
    Intersections intersections;
    for (const auto &object : this->objects) {
      intersections.addAll(object->intersect(ray));
    }
    intersections.sort();
    return intersections;
  }
  Color World::shadeHit(const Computations &computations, int remaining) {
    bool shadowed = isShadowed(computations.overPoint);
    auto surface = lighting(computations.object, this->light.value(), computations.overPoint,
                            computations.eyeVector, computations.normalVector, shadowed);
    auto reflected = this->reflectedColorAt(computations, remaining);
    auto refracted = this->refractedColorAt(computations, remaining);
    auto material = computations.object->material;
    if (material.reflective > 0.0 && material.transparency > 0.0) {
      auto reflectance = Computations::schlick(computations);
      return surface + reflected * reflectance + refracted * (1 - reflectance);
    }
    return surface + reflected + refracted;
  }
  Color World::colorAt(Ray ray, int remaining) {
    Intersections intersections = this->intersect(ray);
    std::optional<Intersection> hit = intersections.hit();
    if (!hit.has_value()) {
      return {0.0, 0.0, 0.0};
    }
    return shadeHit(hit.value().prepareComputations(ray, intersections), remaining);
  }
  Color World::reflectedColorAt(const Computations &computations, int remaining) {
    if (computations.object->material.reflective == 0.0 || remaining == 0) {
      return BLACK;
    }
    auto reflectRay = Ray(computations.overPoint, computations.reflectionVector);
    return this->colorAt(reflectRay, remaining - 1) * computations.object->material.reflective;
  }
  Color World::refractedColorAt(const Computations &computations, int remaining) {
    if (computations.object->material.transparency == 0.0 || remaining == 0) {
      return BLACK;
    }
    auto nRatio = computations.n1 / computations.n2;
    auto cosI = computations.eyeVector.dot(computations.normalVector);
    auto sin2T = pow(nRatio, 2) * (1 - pow(cosI, 2));
    if (sin2T > 1.0) {
      return BLACK;
    }
    auto cosT = sqrt(1.0 - sin2T);
    auto direction
        = computations.normalVector * (nRatio * cosI - cosT) - computations.eyeVector * nRatio;
    auto refractRay = Ray(computations.underPoint, direction);
    auto color = colorAt(refractRay, remaining - 1) * computations.object->material.transparency;
    return color;
  }
  bool World::isShadowed(Tuple point) {
    auto distance = (light->position - point).magnitude();
    auto direction = (light->position - point).normalize();
    auto ray = Ray(point, direction);
    auto hit = intersect(ray).hit();
    if (hit.has_value() && hit.value().t < distance && hit.value().object->material.castShadow) {
      return true;
    }
    return false;
  }
}  // namespace raytracerchallenge