#pragma once

#include <raytracerchallenge/base/Ray.h>

#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace raytracerchallenge {
  class Shape;
  class Computations;
  class Intersection;
  class Intersections;
  /**
   * @brief Represents an intersection between a ray an object
   */
  class Intersection {
  public:
    /**
     * @brief the point on the ray where it intersected with an object
     */
    double t{};
    double u{};
    double v{};
    /**
     * @brief the object that intersected with the ray
     */
    std::shared_ptr<Shape> object;
    /**
     * @brief Construct a new Intersection
     * @param t the point where this intersection occurred on a ray
     * @param object the object which intersected with the ray
     */
    Intersection(double t, std::shared_ptr<Shape> object);
    /**
     * @brief Default constructor
     */
    Intersection();
    /**
     * @brief Prepare ray intersection computations
     * @param ray
     * @return Computations
     */
    [[nodiscard]] Computations prepareComputations(Ray ray) const;
    /**
     * @brief Prepare ray intersection computations
     * @param ray
     * @return Computations
     */
    [[nodiscard]] Computations prepareComputations(Ray ray,
                                                   const Intersections &intersections) const;
    /**
     * @brief Equality operator
     * @param intersection intersection to compare with this one
     * @return true if the intersections have the same properties
     */
    bool operator==(const Intersection &intersection) const;
    /**
     * @brief Less than operator. Comparison is based on the 't' property
     * @param intersection intersection to compare with this one
     * @return true if the t property of this intersection is smaller than that of the argument
     */
    bool operator<(const Intersection &intersection) const;
  };
  /**
   * @brief A collection of Intersection objects
   */
  class Intersections {
  public:
    /**
     * @brief Return the smallest non-negative intersection in the collection
     * @return the smallest non-negative intersection in the collection
     */
    [[nodiscard]] std::optional<Intersection> hit() const;
    /**
     * @brief Construct a new Intersections
     * @param intersections to be wrapped by this container
     */
    explicit Intersections(std::vector<Intersection> intersections);
    /**
     * @brief Default constructor
     */
    Intersections();
    /**
     * @brief Index operator
     * @param x index
     * @return intersection at index x
     */
    Intersection operator[](unsigned int x) const;
    /**
     * @brief Add the content of an Intersections to this one.
     * @param newIntersections Intersections to be added.
     */
    void addAll(Intersections newIntersections);
    /**
     * @brief Sort the intersections
     */
    void sort();
    /**
     * @brief Return the number of intersections in the collection
     * @return number of intersections in the collection
     */
    [[nodiscard]] size_t size() const;

    std::vector<Intersection> intersections;
  };
}  // namespace raytracerchallenge