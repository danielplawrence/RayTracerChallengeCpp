#pragma once

#include <raytracerchallenge/base/Matrix.h>
#include <raytracerchallenge/base/Ray.h>
#include <raytracerchallenge/base/Tuple.h>

namespace raytracerchallenge {
  /**
   * @brief Describes the bounding box of an object
   */
  class BoundingBox {
  public:
    /* Point representing the minimum bound */
    Tuple min = Tuple::point(INFINITY, INFINITY, INFINITY);
    /* Point representing the maximum bound */
    Tuple max = Tuple::point(double(-INFINITY), double(-INFINITY), double(-INFINITY));
    /**
     * @brief Default constructor
     */
    BoundingBox() = default;
    /**
     * @brief Constructor from min and max points
     * @param min Min point of the box
     * @param max Max point of the box
     */
    BoundingBox(Tuple min, Tuple max) {
      this->min = min;
      this->max = max;
    }
    /**
     * @brief Add a new point to the box, extending
     * min and max bounds as necessary
     */
    void add(Tuple point);
    /**
     * @brief Add a new box to the box, extending
     * min and max bounds as necessary
     * @param box
     */
    void add(BoundingBox box);
    /**
     * @brief Returns true if the provided point
     * is inside the bounding box
     * @param point
     * @return true if point is inside this box
     */
    [[nodiscard]] bool contains(Tuple point) const;
    /**
     * @brief Returns true if the provided box
     * is contained within this box
     * @param box
     * @return true if box is inside this box
     */
    [[nodiscard]] bool contains(BoundingBox box) const;
    /**
     * @brief Apply a transform to this box
     * @param matrix Transformation matrix
     * @return Transformed box
     */
    [[nodiscard]] BoundingBox transform(const Matrix &matrix) const;
    /**
     * @brief Check if a ray intersects this boc
     * @param ray
     * @return True if ray intersects this box
     */
    [[nodiscard]] bool intersects(Ray ray) const;
    /**
     * Split this box in two
     * @return A vector, where the first element is the
     * left-hand box and the second is the right-hand box
     */
    std::vector<BoundingBox> split();
  };
}  // namespace raytracerchallenge