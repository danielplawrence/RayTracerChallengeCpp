#include <raytracerchallenge/base/Intersections.h>
#include <raytracerchallenge/shapes/Group.h>

namespace raytracerchallenge {
  void Group::add(const std::shared_ptr<Shape>& object) {
    object->parent = this->sharedPtr;
    object->setMaterial(material);
    this->objects.push_back(object);
    auto cbox = object->parentSpaceBounds();
    this->currentBounds.add(cbox);
  }
  void Group::setMaterial(std::shared_ptr<Material>& newMaterial) {
    this->material = newMaterial;
    for (const auto& obj : this->objects) {
      obj->setMaterial(newMaterial);
    }
  }
  void Group::remove(std::shared_ptr<Shape> object) {
    this->objects.erase(std::remove(this->objects.begin(), this->objects.end(), object),
                        this->objects.end());
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
    for (auto& object : this->objects) {
      xs.addAll(object->intersect(ray));
    }
    xs.sort();
    return xs;
  }
  BoundingBox Group::bounds() { return currentBounds; }
  bool Group::includes(const Shape& object) const {
    return std::any_of(this->objects.cbegin(), this->objects.cend(),
                       [&object](auto target) { return target->includes(object); });
  }
  std::vector<std::vector<std::shared_ptr<Shape>>> Group::partitionChildren() {
    auto halves = this->bounds().split();
    auto left = halves[0];
    auto right = halves[1];
    std::vector<std::shared_ptr<Shape>> leftShapes;
    std::vector<std::shared_ptr<Shape>> rightShapes;
    for (const auto& object : this->objects) {
      if (left.contains(object->parentSpaceBounds())) {
        leftShapes.emplace_back(object);
      } else if (right.contains(object->parentSpaceBounds())) {
        rightShapes.emplace_back(object);
      }
    }
    std::for_each(leftShapes.begin(), leftShapes.end(), [&](auto shape) { this->remove(shape); });
    std::for_each(rightShapes.begin(), rightShapes.end(), [&](auto shape) { this->remove(shape); });
    return {leftShapes, rightShapes};
  }
  void Group::makeSubgroup(const std::vector<std::shared_ptr<Shape>>& shapes) {
    auto subGroup = new Group();
    for (const auto& shape : shapes) {
      subGroup->add(shape);
    }
    this->add(subGroup->sharedPtr);
  }
  void Group::divide(unsigned int threshold) {
    if (threshold <= this->objects.size()) {
      auto leftRight = this->partitionChildren();
      if (!leftRight[0].empty()) {
        this->makeSubgroup(leftRight[0]);
      }
      if (!leftRight[1].empty()) {
        this->makeSubgroup(leftRight[1]);
      }
    }
    for (const auto& shape : this->objects) {
      shape->divide(threshold);
    }
  }
}  // namespace raytracerchallenge