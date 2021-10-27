#pragma once

#include <raytracerchallenge/base/Computations.h>
#include <raytracerchallenge/base/Light.h>
#include <raytracerchallenge/shapes/Shape.h>

#include <optional>

namespace raytracerchallenge {
  class World {
  public:
    std::vector<std::shared_ptr<Shape>> objects;
    std::optional<PointLight> light;
    /**
     * Default constructor for a World
     */
    World();
    /**
     * Return true if the world contains no objects
     * @return true if the world contains no objects
     */
    [[nodiscard]] bool isEmpty() const;
    /**
     * Add an Object to the world
     * @param object target Object
     */
    void add(const std::shared_ptr<Shape> &object);
    /**
     * Intersect this world with a ray
     * @param ray to pass through the world
     * @return intersections between the ray
     * and objects in this world
     */
    Intersections intersect(Ray ray);
    /**
     * @brief Return the default World
     * @return Default World
     */
    static World defaultWorld();
    /**
     * @brief Return the color at the intersection encapsulated
     * by computations in this world
     * @param computations
     * @return color
     */
    Color shadeHit(const Computations &computations, int remaining);
    /**
     * Intersect the world with the given ray and return
     * the color at the resulting intersection
     * @param ray Ray to intersect with this World
     * @return Color at the resulting intersection
     */
    Color colorAt(Ray ray, int remaining);
    /**
     * Calculate the reflected color for a set of Computations
     * @param computations
     * @param remaining recursion limit
     * @return reflected color
     */
    Color reflectedColorAt(const Computations &computations, int remaining);
    /**
     * Calculated the refracted color for a set of Computations
     * @param computations
     * @param remaining recursion limit
     * @return refracted Color
     */
    Color refractedColorAt(const Computations &computations, int remaining);
    /**
     * Return True if this point is in shadow
     * @param point to check for shadow
     * @return True if this point is in shadow
     */
    bool isShadowed(Tuple point);
  };
}  // namespace raytracerchallenge