#pragma once

#include <raytracerchallenge/base/BoundingBox.h>
#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/base/Material.h>
#include <raytracerchallenge/base/Matrix.h>
#include <raytracerchallenge/base/Ray.h>

namespace raytracerchallenge {
  /**
   * @brief Base class for objects
   */
  class Shape {
  public:
    /**
     * @brief Default constructor
     */
    Shape() {
      this->transform = Matrix::identity(4);
      this->sharedPtr = std::shared_ptr<Shape>(this);
    }
    /**
     * @brief Transformation matrix of this object
     */
    Matrix transform;
    /**
     * @brief Material for this object
     */
    Material material;
    /**
     * @brief The parent of this object
     */
    std::shared_ptr<Shape> parent = nullptr;
    /**
     * @brief Return all intersections between this object
     * and the provided ray
     * @param ray
     * @return collection of Intersections representing positions where
     * the ray passed through this object
     */
    Intersections intersect(Ray ray) {
      Ray transformed = ray.transform(this->transform.inverse());
      return localIntersect(transformed);
    }
    /**
     * @brief Implementation-specific logic for returning all intersections
     * between a shape and a ray. Defaults to Sphere behavior.
     * @param ray
     * @return collection of Intersections representing positions where
     * the ray passed through this object
     */
    virtual Intersections localIntersect(Ray ray) = 0;
    /**
     * @brief Return the normal vector at the specified point on an object
     * @param point
     * @return normal vector
     */
    Tuple normalAt(Tuple point, Intersection hit) {
      auto localPoint = this->worldToObject(point);
      auto localNormal = this->localNormalAt(localPoint, std::move(hit));
      return this->normalToWorld(localNormal);
    }
    /**
     * @brief Implementation-specific logic for returning the normal at a specific
     * point on this object.
     * @param point
     * @return the normal vector at the specified point on this object
     */
    virtual Tuple localNormalAt(Tuple point, Intersection hit) = 0;
    /**
     * @brief Return the bounds for this shape
     * @return bounds for this shape
     */
    virtual BoundingBox bounds() = 0;
    /**
     * @brief Return the bounding box of this object in parent space
     * @return The bounding box of this object
     */
    BoundingBox parentSpaceBounds() { return this->bounds().transform(this->transform); }
    /**
     * Helper for converting from world space to object space
     * @param point Point in world space
     * @return point in object space
     */
    [[nodiscard]] Tuple worldToObject(Tuple point) const {
      Tuple p = point;
      if (this->parent != nullptr) {
        p = this->parent->worldToObject(p);
      }
      return this->transform.inverse() * p;
    }
    /**
     * Helper for converting a normal vector in object space to
     * a normal vector in world space
     * @param normal Normal vector in object space
     * @return Normal vector in world space
     */
    [[nodiscard]] Tuple normalToWorld(Tuple normal) const {
      normal = this->transform.inverse().transposed() * normal;
      normal.w = 0.0;
      normal = normal.normalize();
      if (this->parent != nullptr) {
        normal = this->parent->normalToWorld(normal);
      }
      return normal;
    }
    /**
     * @brief Equality operator. Objects are equal if they share the same properties
     * @param object object to compare with this one
     * @return true if the objects share the same properties
     */
    bool operator==(const Shape &object) const {
      return this->transform == object.transform && this->material == object.material;
    }
    /**
     * @brief Less than operator; just uses the ID
     * @param object object to compare with this one
     * @return true if this object is less than the target
     */
    bool operator<(const Shape &object) const { return this->sharedPtr < object.sharedPtr; }
    /**
     * @brief Return true if the objects share the same ID
     * @param object Object for comparison
     * @return true if the objects share the same ID
     */
    [[nodiscard]] bool is(const Shape &object) const { return this->sharedPtr == object.sharedPtr; }
    /**
     * Return true if this shape includes another shape.
     * The default definition of 'includes' is equality,
     * but subtypes may differ in their implementation.
     * @param object Object for comparison
     * @return true if this shape includes the target.
     */
    [[nodiscard]] virtual bool includes(const Shape &object) const { return this->is(object); }
    virtual ~Shape() = default;
    std::shared_ptr<Shape> sharedPtr;
  };
}  // namespace raytracerchallenge