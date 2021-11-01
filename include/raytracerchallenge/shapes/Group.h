#pragma once

#include <raytracerchallenge/shapes/Shape.h>

namespace raytracerchallenge {
  /**
   * @brief Represents a group of objects
   */
  class Group : public Shape {
  public:
    /* Objects contained in this group */
    std::vector<std::shared_ptr<Shape>> objects;
    /**
     * @brief Factory method
     * @return a pointer to a new Group
     */
    static std::shared_ptr<Shape> create() {
      auto shape = new Group();
      return shape->sharedPtr;
    }
    /**
     * @brief Add an object to the group
     * @param object pointer to target object
     */
    void add(const std::shared_ptr<Shape>& object);
    void remove(std::shared_ptr<Shape> object);
    BoundingBox bounds() override;
    Tuple localNormalAt(Tuple point, Intersection hit) override;
    Intersections localIntersect(Ray ray) override;
    [[nodiscard]] bool includes(const Shape& object) const override;
    std::vector<std::vector<std::shared_ptr<Shape>>> partitionChildren();
    void makeSubgroup(const std::vector<std::shared_ptr<Shape>>& shapes);
    void divide(unsigned int threshold) override;
    void setMaterial(std::shared_ptr<Material>& newMaterial) override;

  private:
    BoundingBox currentBounds;
  };
}  // namespace raytracerchallenge