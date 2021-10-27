#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/CSG.h>

namespace raytracerchallenge {
  Tuple CSG::localNormalAt(Tuple point, class Intersection hit) {
    (void)point;
    (void)hit;
    return {};
  }
  BoundingBox CSG::bounds() {
    auto box = BoundingBox();
    box.add(this->left->parentSpaceBounds());
    box.add(this->right->parentSpaceBounds());
    return box;
  }
  Intersections CSG::localIntersect(Ray ray) {
    auto xs = Intersections();
    if (!this->bounds().intersects(ray)) {
      return xs;
    }
    xs.addAll(this->left->intersect(ray));
    xs.addAll(this->right->intersect(ray));
    xs.sort();
    return filterIntersections(xs);
  }
  bool CSG::intersectionAllowed(CSG::Operation op, bool leftHit, bool inLeft, bool inRight) {
    switch (op) {
      case Union:
        return (leftHit && !inRight) || (!leftHit && !inLeft);
      case Intersection:
        return (leftHit && inRight) || (!leftHit && inLeft);
      case Difference:
        return (leftHit && !inRight) || (!leftHit && inLeft);
      default:
        return false;
    }
  }
  Intersections CSG::filterIntersections(const Intersections &intersections) const {
    bool inLeft = false;
    bool inRight = false;
    auto result = Intersections();
    for (const auto &intersection : intersections.intersections) {
      auto lhit = this->left->includes(*intersection.object);
      if (intersectionAllowed(this->operation, lhit, inLeft, inRight)) {
        result.intersections.push_back(intersection);
      }
      if (lhit) {
        inLeft = !inLeft;
      } else {
        inRight = !inRight;
      }
    }
    return result;
  }
  bool CSG::includes(const Shape &object) const {
    return this->left->includes(object) || this->right->includes(object);
  }
}  // namespace raytracerchallenge