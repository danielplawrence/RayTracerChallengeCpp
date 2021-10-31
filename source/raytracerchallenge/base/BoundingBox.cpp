#include <raytracerchallenge/Constants.h>
#include <raytracerchallenge/base/BoundingBox.h>

namespace raytracerchallenge {
  void BoundingBox::add(Tuple point) {
    if (point.x < this->min.x) {
      this->min.x = point.x;
    }
    if (point.y < this->min.y) {
      this->min.y = point.y;
    }
    if (point.z < this->min.z) {
      this->min.z = point.z;
    }
    if (point.x > this->max.x) {
      this->max.x = point.x;
    }
    if (point.y > this->max.y) {
      this->max.y = point.y;
    }
    if (point.z > this->max.z) {
      this->max.z = point.z;
    }
  }
  void BoundingBox::add(BoundingBox box) {
    this->add(box.min);
    this->add(box.max);
  }
  bool BoundingBox::contains(Tuple point) const {
    return point.x >= this->min.x && point.x <= this->max.x && point.y >= this->min.y
           && point.y <= this->max.y && point.z >= this->min.z && point.z <= this->max.z;
  }
  bool BoundingBox::contains(BoundingBox box) const {
    return this->contains(box.max) && this->contains(box.min);
  }
  std::array<double, 2> checkBoxAxis(double origin, double direction, double min = -1.0,
                                     double max = 1.0) {
    auto tminNumerator = (min - origin);
    auto tmaxNumerator = (max - origin);
    double tmin;
    double tmax;
    if (abs(direction) >= EPS) {
      tmin = tminNumerator / direction;
      tmax = tmaxNumerator / direction;
    } else {
      tmin = tminNumerator * INFINITY;
      tmax = tmaxNumerator * INFINITY;
    }
    if (tmin > tmax) {
      std::swap(tmin, tmax);
    }
    return {tmin, tmax};
  }
  BoundingBox BoundingBox::transform(const Matrix &matrix) const {
    auto p1 = this->min;
    auto p2 = Tuple(this->min.x, this->min.y, this->max.z, 1.0);
    auto p3 = Tuple(this->min.x, this->max.y, this->min.z, 1.0);
    auto p4 = Tuple(this->min.x, this->max.y, this->max.z, 1.0);
    auto p5 = Tuple(this->max.x, this->min.y, this->min.z, 1.0);
    auto p6 = Tuple(this->max.x, this->min.y, this->max.z, 1.0);
    auto p7 = Tuple(this->min.x, this->min.y, this->max.z, 1.0);
    auto p8 = Tuple(this->max.x, this->max.y, this->min.z, 1.0);
    auto newBox = BoundingBox();
    for (Tuple p : {p1, p2, p3, p4, p5, p6, p7, p8}) {
      newBox.add(matrix * p);
    }
    return newBox;
  }
  bool BoundingBox::intersects(Ray ray) const {
    auto xMinMax = checkBoxAxis(ray.origin.x, ray.direction.x, this->min.x, this->max.x);
    auto yMinMax = checkBoxAxis(ray.origin.y, ray.direction.y, this->min.y, this->max.y);
    auto zMinMax = checkBoxAxis(ray.origin.z, ray.direction.z, this->min.z, this->max.z);
    auto tMin = std::max({xMinMax[0], yMinMax[0], zMinMax[0]});
    auto tMax = std::min({xMinMax[1], yMinMax[1], zMinMax[1]});
    if (tMin > tMax) {
      return false;
    }
    if (tMin > 0 || tMax > 0) {
      return true;
    }
    return false;
  }
  std::vector<BoundingBox> BoundingBox::split() {
    auto dx = abs(this->max.x - this->min.x);
    auto dy = abs(this->max.y - this->min.y);
    auto dz = abs(this->max.z - this->min.z);
    auto greatest = std::max({dx, dy, dz});
    auto x0 = this->min.x;
    auto y0 = this->min.y;
    auto z0 = this->min.z;
    auto x1 = this->max.x;
    auto y1 = this->max.y;
    auto z1 = this->max.z;
    if (greatest == dx) {
      x0 = x0 + dx / 2.0;
      x1 = x0;
    } else if (greatest == dy) {
      y0 = y0 + dy / 2.0;
      y1 = y0;
    } else {
      z0 = z0 + dz / 2.0;
      z1 = z0;
    }
    auto midMin = Tuple(x0, y0, z0, 1.0);
    auto midMax = Tuple(x1, y1, z1, 1.0);
    auto left = BoundingBox(this->min, midMax);
    auto right = BoundingBox(midMin, this->max);
    return {left, right};
  }
}  // namespace raytracerchallenge