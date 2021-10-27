#pragma once

#include <raytracerchallenge/shapes/Shape.h>

namespace raytracerchallenge {
  /**
   * @brief Constructive Solid Geometry -- used
   * to combine shapes using set operations
   */
  class CSG : public Shape {
  public:
    enum Operation { Union, Intersection, Difference };
    Operation operation;
    std::shared_ptr<Shape> left;
    std::shared_ptr<Shape> right;
    CSG(std::shared_ptr<Shape> &shape1, std::shared_ptr<Shape> &shape2, Operation operation) {
      this->left = shape1;
      this->right = shape2;
      shape1->parent = this->sharedPtr;
      shape2->parent = this->sharedPtr;
      this->operation = operation;
    }
    static std::shared_ptr<Shape> create(std::shared_ptr<Shape> &shape1,
                                         std::shared_ptr<Shape> &shape2,
                                         Operation operation = Union) {
      return (new CSG(shape1, shape2, operation))->sharedPtr;
    }
    Tuple localNormalAt(Tuple point, class Intersection hit) override;
    Intersections localIntersect(Ray ray) override;
    BoundingBox bounds() override;
    [[nodiscard]] Intersections filterIntersections(const Intersections &intersections) const;
    [[nodiscard]] bool includes(const Shape &object) const override;
    static bool intersectionAllowed(Operation op, bool leftHit, bool inLeft, bool inRight);
  };
}  // namespace raytracerchallenge