#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Group.h>

namespace raytracerchallenge {
  void Group::add(const std::shared_ptr<Shape> &object) {
    object->parent = this->sharedPtr;
    this->objects.push_back(object);
  }
  Tuple Group::localNormalAt(Tuple point, Intersection hit) {
    (void)hit;
    (void)point;
    return {};
  }
  Intersections Group::localIntersect(Ray ray) {
    auto xs = Intersections();
    if (!this->bounds().intersects(ray)) {
      return xs;
    }
    for (auto &object : this->objects) {
      xs.addAll(object->intersect(ray));
    }
    xs.sort();
    return xs;
  }
  BoundingBox Group::bounds() {
    auto box = BoundingBox();
    for (auto &object : this->objects) {
      auto cbox = object->parentSpaceBounds();
      box.add(cbox);
    }
    return box;
  }
  bool Group::includes(const Shape &object) const {
    return std::any_of(this->objects.cbegin(), this->objects.cend(),
                       [&object](auto target) { return target->includes(object); });
  }
}  // namespace raytracerchallenge