#define _USE_MATH_DEFINES
#include <fmt/format.h>
#include <raytracerchallenge/raytracerchallenge.h>

#include <Eigen/Dense>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <regex>
#include <thread>
#include <utility>

using Eigen::MatrixXd;
using Eigen::VectorXd;

using namespace raytracerchallenge;

const double EPS = 0.0001;
const double NEGATIVE_INFINITY = -1.0 * INFINITY;

/**
 * Execute the provided function in parallel
 * @param nElements Number of elements which will be iterated over
 * @param functor Function <int start, int end> which will process a given chunk of the loop
 */
static void parallelFor(int nElements, std::function<void(int start, int end)> functor) {
  int nbThreadsHint = (int)std::thread::hardware_concurrency();
  int nbThreads = nbThreadsHint == 0 ? 8 : (nbThreadsHint);
  int batchSize = nElements / nbThreads;
  int batchRemainder = nElements % nbThreads;
  std::vector<std::thread> threads(nbThreads);
  for (int i = 0; i < nbThreads; ++i) {
    int start = i * batchSize;
    threads[i] = std::thread(functor, start, start + batchSize);
  }
  int start = (int)(nbThreads * batchSize);
  int end = (int)start + batchRemainder;
  functor(start, end);
  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

RayTracerChallenge::Tuple::Tuple() {
  this->x = 0.0;
  this->y = 0.0;
  this->z = 0.0;
  this->w = 0.0;
}
RayTracerChallenge::Tuple::Tuple(double x, double y, double z, double w) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}
bool RayTracerChallenge::Tuple::isVector() const { return w == 0; }
double RayTracerChallenge::Tuple::magnitude() const {
  return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2) + pow(this->w, 2));
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::normalize() const {
  return {this->x / this->magnitude(), this->y / this->magnitude(), this->z / this->magnitude(),
          this->w / this->magnitude()};
}
bool RayTracerChallenge::Tuple::operator==(const RayTracerChallenge::Tuple &t) const {
  return doubleEquals(t.x, this->x) && doubleEquals(t.y, this->y) && doubleEquals(t.z, this->z)
         && doubleEquals(t.w, this->w);
}
double RayTracerChallenge::Tuple::dot(const RayTracerChallenge::Tuple &t) const {
  return this->x * t.x + this->y * t.y + this->z * t.z + this->w * t.w;
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::cross(
    const RayTracerChallenge::Tuple &t) const {
  return RayTracerChallenge::Tuple::vector(
      this->y * t.z - this->z * t.y, this->z * t.x - this->x * t.z, this->x * t.y - this->y * t.x);
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::reflect(
    const RayTracerChallenge::Tuple &t) const {
  return *this - t * 2.0 * this->dot(t);
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator+(
    const RayTracerChallenge::Tuple &t) const {
  return {t.x + this->x, t.y + this->y, t.z + this->z, t.w + this->w};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator-(
    const RayTracerChallenge::Tuple &t) const {
  return {this->x - t.x, this->y - t.y, this->z - t.z, this->w - t.w};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator-() const {
  return {-this->x, -this->y, -this->z, -this->w};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator*(double f) const {
  return {f * this->x, f * this->y, f * this->z, f * this->w};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator/(double f) const {
  return {this->x / f, this->y / f, this->z / f, this->w / f};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::point(double x, double y, double z) {
  return {x, y, z, 1.0};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::vector(double x, double y, double z) {
  return {x, y, z, 0.0};
}
bool RayTracerChallenge::Tuple::doubleEquals(double x, double y) {
  if (x == INFINITY && y == INFINITY) {
    return true;
  }
  if (x == -1.0 * INFINITY && y == -1.0 * INFINITY) {
    return true;
  }
  return abs(x - y) < EPS;
}

RayTracerChallenge::Color::Color() {
  this->red = 0.0;
  this->green = 0.0;
  this->blue = 0.0;
}
RayTracerChallenge::Color::Color(double red, double green, double blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
}
RayTracerChallenge::Color RayTracerChallenge::Color::operator*(double f) const {
  Tuple t = Tuple(this->red, this->green, this->blue, 0) * f;
  return {t.x, t.y, t.z};
}
RayTracerChallenge::Color RayTracerChallenge::Color::operator-(
    const RayTracerChallenge::Color &c) const {
  Tuple t1 = Tuple(this->red, this->green, this->blue, 0);
  Tuple t2 = Tuple(c.red, c.green, c.blue, 0);
  Tuple t3 = t1 - t2;
  return {t3.x, t3.y, t3.z};
}
RayTracerChallenge::Color RayTracerChallenge::Color::operator+(
    const RayTracerChallenge::Color &c) const {
  Tuple t1 = Tuple(this->red, this->green, this->blue, 0);
  Tuple t2 = Tuple(c.red, c.green, c.blue, 0);
  Tuple t3 = t1 + t2;
  return {t3.x, t3.y, t3.z};
}
RayTracerChallenge::Color RayTracerChallenge::Color::operator*(
    const RayTracerChallenge::Color &c) const {
  return {this->red * c.red, this->green * c.green, this->blue * c.blue};
}
bool RayTracerChallenge::Color::operator==(const RayTracerChallenge::Color &c) const {
  Tuple t1 = Tuple(this->red, this->green, this->blue, 0);
  Tuple t2 = Tuple(c.red, c.green, c.blue, 0);
  return t1 == t2;
}
const RayTracerChallenge::Color RayTracerChallenge::Color::BLACK = {0.0, 0.0, 0.0};
const RayTracerChallenge::Color RayTracerChallenge::Color::WHITE = {1.0, 1.0, 1.0};
RayTracerChallenge::Canvas::Canvas(int width, int height) {
  this->width = width;
  this->height = height;
  this->pixels = std::vector<std::vector<Color>>(width);
  for (int x = 0; x < width; x++) {
    this->pixels[x] = std::vector<Color>(height);
  }
}
void RayTracerChallenge::Canvas::writePixel(int x, int y, RayTracerChallenge::Color c) {
  this->pixels[x][y] = c;
}
RayTracerChallenge::Color RayTracerChallenge::Canvas::pixelAt(int x, int y) {
  return this->pixels[x][y];
}

std::string RayTracerChallenge::Canvas::toPortablePixmap() {
  std::string ppmVariant("P3");
  std::string maxColorValue("255");
  std::string header(ppmVariant + "\n" + fmt::to_string(this->width) + " "
                     + fmt::to_string(this->height) + "\n" + maxColorValue + "\n");
  for (int y = 0; y < this->height; y++) {
    std::string line;
    for (int x = 0; x < this->width; x++) {
      Color c = this->pixels[x][y];
      std::vector<double> colorVals{c.red, c.green, c.blue};
      std::for_each(colorVals.begin(), colorVals.end(), [&line, &header](double f) {
        std::string val
            = fmt::to_string(std::ceil(std::clamp(float(f * 255.0), float(0.0), float(255.0))));
        if (line.size() + val.size() > 70) {
          line.pop_back();
          header += line + "\n";
          line = "";
        }
        line += val + " ";
      });
    }
    line.pop_back();
    header += line + "\n";
  }
  return header;
}

RayTracerChallenge::Matrix::Matrix(unsigned int x, unsigned int y,
                                   std::vector<std::vector<double>> m) {
  MatrixXd mat(x, y);
  for (auto i = 0U; i < x; i++) {
    mat.row(i) = VectorXd::Map(&m[i][0], (long)m[i].size());
  }
  this->m = mat;
}
RayTracerChallenge::Matrix::Matrix() = default;
RayTracerChallenge::Matrix::Matrix(Eigen::MatrixXd m) { this->m = std::move(m); }
bool RayTracerChallenge::Matrix::operator==(const Matrix &matrix) const {
  return this->m.isApprox(matrix.m, EPS);
}
bool RayTracerChallenge::Matrix::operator!=(const Matrix &matrix) const {
  return this->m != matrix.m;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::operator*(
    const RayTracerChallenge::Matrix &matrix) const {
  return Matrix(this->m * matrix.m);
}
RayTracerChallenge::Tuple RayTracerChallenge::Matrix::operator*(
    const RayTracerChallenge::Tuple &tuple) const {
  Tuple res = Tuple(0, 0, 0, 0);
  res.x = Tuple(this->m(0, 0), this->m(0, 1), this->m(0, 2), this->m(0, 3)).dot(tuple);
  res.y = Tuple(this->m(1, 0), this->m(1, 1), this->m(1, 2), this->m(1, 3)).dot(tuple);
  res.z = Tuple(this->m(2, 0), this->m(2, 1), this->m(2, 2), this->m(2, 3)).dot(tuple);
  res.w = Tuple(this->m(3, 0), this->m(3, 1), this->m(3, 2), this->m(3, 3)).dot(tuple);
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::identity(unsigned int size) {
  return Matrix(MatrixXd::Identity(size, size));
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::transposed() {
  return Matrix(this->m.transpose());
}

double RayTracerChallenge::Matrix::determinant() const { return this->m.determinant(); }
bool RayTracerChallenge::Matrix::invertible() const { return determinant() != 0.0; }
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::inverse() const {
  return Matrix(this->m.inverse());
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::translation(double x, double y, double z) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m(0, 3) = x;
  res.m(1, 3) = y;
  res.m(2, 3) = z;
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::scaling(double x, double y, double z) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m(0, 0) = x;
  res.m(1, 1) = y;
  res.m(2, 2) = z;
  res.m(3, 3) = 1;
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotationX(double radians) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m(1, 1) = cos(radians);
  res.m(1, 2) = -sin(radians);
  res.m(2, 1) = sin(radians);
  res.m(2, 2) = cos(radians);
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotationY(double radians) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m(0, 0) = cos(radians);
  res.m(0, 2) = sin(radians);
  res.m(2, 0) = -sin(radians);
  res.m(2, 2) = cos(radians);
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotationZ(double radians) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m(0, 0) = cos(radians);
  res.m(0, 1) = -sin(radians);
  res.m(1, 0) = sin(radians);
  res.m(1, 1) = cos(radians);
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::shearing(double xy, double xz, double yx,
                                                                double yz, double zx, double zy) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m(0, 1) = xy;
  res.m(0, 2) = xz;
  res.m(1, 0) = yx;
  res.m(1, 2) = yz;
  res.m(2, 0) = zx;
  res.m(2, 1) = zy;
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::translated(double x, double y,
                                                                  double z) const {
  RayTracerChallenge::Matrix t = translation(x, y, z);
  return t * *this;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::scaled(double x, double y, double z) const {
  RayTracerChallenge::Matrix t = scaling(x, y, z);
  return t * *this;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotatedX(double radians) const {
  RayTracerChallenge::Matrix t = rotationX(radians);
  return t * *this;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotatedY(double radians) const {
  RayTracerChallenge::Matrix t = rotationY(radians);
  return t * *this;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotatedZ(double radians) const {
  RayTracerChallenge::Matrix t = rotationZ(radians);
  return t * *this;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::sheared(double xy, double xz, double yx,
                                                               double yz, double zx,
                                                               double zy) const {
  RayTracerChallenge::Matrix t = shearing(xy, xz, yx, yz, zx, zy);
  return t * *this;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::view(RayTracerChallenge::Tuple from,
                                                            RayTracerChallenge::Tuple to,
                                                            RayTracerChallenge::Tuple up) {
  auto forward = (to - from).normalize();
  auto upN = up.normalize();
  auto left = forward.cross(upN);
  auto trueUp = left.cross(forward);
  RayTracerChallenge::Matrix orientation(4, 4,
                                         {{left.x, left.y, left.z, 0.0},
                                          {trueUp.x, trueUp.y, trueUp.z, 0.0},
                                          {-forward.x, -forward.y, -forward.z, 0.0},
                                          {0.0, 0.0, 0.0, 1.0}});
  return orientation * translation(-from.x, -from.y, -from.z);
}
RayTracerChallenge::Ray::Ray(RayTracerChallenge::Tuple origin,
                             RayTracerChallenge::Tuple direction) {
  this->origin = origin;
  this->direction = direction;
}
RayTracerChallenge::Tuple RayTracerChallenge::Ray::position(double t) const {
  return RayTracerChallenge::Tuple(this->origin + this->direction * t);
}
RayTracerChallenge::Ray RayTracerChallenge::Ray::transform(
    const RayTracerChallenge::Matrix &matrix) const {
  return {matrix * this->origin, matrix * this->direction};
}
RayTracerChallenge::Shape::Shape() {
  uint64_t current_time_us = std::chrono::duration_cast<std::chrono::microseconds>(
                                 std::chrono::high_resolution_clock::now().time_since_epoch())
                                 .count();
  this->id = fmt::to_string(current_time_us);
  this->transform = Matrix::identity(4);
  this->sharedPtr = std::shared_ptr<Shape>(this);
}
RayTracerChallenge::Intersections RayTracerChallenge::Shape::intersect(
    RayTracerChallenge::Ray ray) {
  RayTracerChallenge::Ray transformed = ray.transform(this->transform.inverse());
  return localIntersect(transformed);
}
RayTracerChallenge::Intersections RayTracerChallenge::Sphere::localIntersect(Ray ray) {
  Tuple sphereToRay = ray.origin - RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
  double a = ray.direction.dot(ray.direction);
  double b = 2.0 * ray.direction.dot(sphereToRay);
  double c = sphereToRay.dot(sphereToRay) - 1.0;
  double discriminant = pow(b, 2.0) - 4.0 * a * c;
  if (discriminant < 0.0) {
    return Intersections(std::vector<RayTracerChallenge::Intersection>(0));
  }
  double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
  double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
  return Intersections(std::vector<RayTracerChallenge::Intersection>{
      RayTracerChallenge::Intersection(t1, this->sharedPtr),
      RayTracerChallenge::Intersection(t2, this->sharedPtr)});
}
bool RayTracerChallenge::Shape::operator==(const Shape &object) const {
  return this->transform == object.transform && this->material == object.material;
}
bool RayTracerChallenge::Shape::includes(const Shape &object) const { return this->is(object); }
bool RayTracerChallenge::Shape::is(const Shape &object) const { return this->id == object.id; }
RayTracerChallenge::Intersection::Intersection(double t, std::shared_ptr<Shape> object) {
  this->t = t;
  this->object = std::move(object);
}
RayTracerChallenge::Intersection::Intersection() = default;
bool RayTracerChallenge::Intersection::operator==(const Intersection &intersection) const {
  return this->object->is(*intersection.object) && this->t == intersection.t;
}
bool RayTracerChallenge::Intersection::operator<(
    const RayTracerChallenge::Intersection &intersection) const {
  return this->t < intersection.t;
}
RayTracerChallenge::Computations RayTracerChallenge::Intersection::prepareComputations(
    RayTracerChallenge::Ray ray) const {
  auto computations = RayTracerChallenge::Computations();
  computations.t = this->t;
  computations.object = this->object;
  computations.point = ray.position(t);
  computations.eyeVector = -ray.direction;
  computations.normalVector = this->object->normalAt(computations.point, *this);
  if (computations.normalVector.dot(computations.eyeVector) < 0.0) {
    computations.inside = true;
    computations.normalVector = -computations.normalVector;
  }
  computations.reflectionVector = ray.direction.reflect(computations.normalVector);
  computations.overPoint = computations.point + (computations.normalVector * EPS);
  computations.underPoint = computations.point - (computations.normalVector * EPS);
  return computations;
}
RayTracerChallenge::Computations RayTracerChallenge::Intersection::prepareComputations(
    RayTracerChallenge::Ray ray, const RayTracerChallenge::Intersections &intersections) const {
  auto computations = prepareComputations(ray);
  std::vector<std::shared_ptr<Shape>> containers;
  for (const Intersection &i : intersections.intersections) {
    if (i == *this) {
      if (containers.empty()) {
        computations.n1 = 1.0;
      } else {
        computations.n1 = containers.back()->material.refractiveIndex;
      }
    }
    auto position = std::find(containers.begin(), containers.end(), i.object);
    if (position != containers.end()) {
      containers.erase(position);
    } else {
      containers.push_back(i.object);
    }
    if (i == *this) {
      if (containers.empty()) {
        computations.n2 = 1.0;
      } else {
        computations.n2 = containers.back()->material.refractiveIndex;
      }
      break;
    }
  }
  return computations;
}
std::optional<RayTracerChallenge::Intersection> RayTracerChallenge::Intersections::hit() const {
  std::vector<RayTracerChallenge::Intersection> nonNegative;
  std::copy_if(intersections.cbegin(), intersections.cend(), std::back_inserter(nonNegative),
               [](const Intersection &i) { return i.t >= 0.0; });
  if (nonNegative.empty()) {
    return {};
  }
  return *std::min_element(nonNegative.begin(), nonNegative.end());
}
RayTracerChallenge::Intersections::Intersections() = default;
void RayTracerChallenge::Intersections::Intersections::addAll(Intersections newIntersections) {
  this->intersections.insert(this->intersections.end(), newIntersections.intersections.begin(),
                             newIntersections.intersections.end());
}
RayTracerChallenge::Intersections::Intersections(std::vector<Intersection> intersections) {
  this->intersections = std::move(intersections);
}
RayTracerChallenge::Intersection RayTracerChallenge::Intersections::operator[](
    unsigned int x) const {
  return this->intersections[x];
}
size_t RayTracerChallenge::Intersections::size() const { return this->intersections.size(); }
void RayTracerChallenge::Intersections::sort() {
  std::sort(this->intersections.begin(), this->intersections.end());
}
RayTracerChallenge::Tuple RayTracerChallenge::Sphere::localNormalAt(Tuple point, Intersection hit) {
  (void)hit;
  return point - RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
}
RayTracerChallenge::BoundingBox RayTracerChallenge::Sphere::bounds() {
  return {{-1.0, -1.0, -1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};
}
RayTracerChallenge::Tuple RayTracerChallenge::Shape::normalAt(Tuple point, Intersection hit) {
  auto localPoint = this->worldToObject(point);
  auto localNormal = this->localNormalAt(localPoint, std::move(hit));
  return this->normalToWorld(localNormal);
}
bool RayTracerChallenge::Shape::operator<(const RayTracerChallenge::Shape &object) const {
  return this->id < object.id;
}
RayTracerChallenge::Tuple RayTracerChallenge::Plane::localNormalAt(Tuple point, Intersection hit) {
  (void)point;
  (void)hit;
  return RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
}
RayTracerChallenge::Intersections RayTracerChallenge::Plane::localIntersect(Ray ray) {
  if (abs(ray.direction.y) < EPS) {
    return {};
  }
  auto t = -ray.origin.y / ray.direction.y;
  return Intersections({RayTracerChallenge::Intersection(t, this->sharedPtr)});
}
RayTracerChallenge::BoundingBox RayTracerChallenge::Plane::bounds() {
  return {Tuple(NEGATIVE_INFINITY, 0.0, NEGATIVE_INFINITY, 1.0),
          Tuple(INFINITY, 0.0, INFINITY, 1.0)};
}
std::array<double, 2> checkAxis(double origin, double direction, double min = -1.0,
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
RayTracerChallenge::Intersections RayTracerChallenge::Cube::localIntersect(Ray ray) {
  auto xMinMax = checkAxis(ray.origin.x, ray.direction.x);
  auto yMinMax = checkAxis(ray.origin.y, ray.direction.y);
  auto zMinMax = checkAxis(ray.origin.z, ray.direction.z);
  auto tMin = std::max({xMinMax[0], yMinMax[0], zMinMax[0]});
  auto tMax = std::min({xMinMax[1], yMinMax[1], zMinMax[1]});
  if (tMin > tMax) {
    return {};
  }
  return Intersections(std::vector<RayTracerChallenge::Intersection>{
      RayTracerChallenge::Intersection(tMin, this->sharedPtr),
      RayTracerChallenge::Intersection(tMax, this->sharedPtr)});
}
RayTracerChallenge::Tuple RayTracerChallenge::Cube::localNormalAt(Tuple point, Intersection hit) {
  (void)hit;
  auto maxC = std::max({abs(point.x), abs(point.y), abs(point.z)});
  if (maxC == abs(point.x)) {
    return {point.x, 0.0, 0.0, 0.0};
  }
  if (maxC == abs(point.y)) {
    return {0.0, point.y, 0.0, 0.0};
  }
  return {0.0, 0.0, point.z, 0.0};
}
RayTracerChallenge::BoundingBox RayTracerChallenge::Cube::bounds() {
  return {{-1.0, -1.0, -1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};
}
RayTracerChallenge::PointLight::PointLight(RayTracerChallenge::Tuple position,
                                           RayTracerChallenge::Color intensity) {
  this->position = position;
  this->intensity = intensity;
}
bool RayTracerChallenge::Material::operator==(const RayTracerChallenge::Material &material) const {
  return this->ambient == material.ambient && this->diffuse == material.diffuse
         && this->specular == material.specular && this->shininess == material.shininess;
}
RayTracerChallenge::Color RayTracerChallenge::lighting(const std::shared_ptr<Shape> &shape,
                                                       RayTracerChallenge::PointLight light,
                                                       RayTracerChallenge::Tuple position,
                                                       RayTracerChallenge::Tuple eyeVector,
                                                       RayTracerChallenge::Tuple normalVector,
                                                       bool inShadow) {
  Color diffuse;
  Color specular;
  Color ambient;
  Color color;
  if (shape->material.pattern != nullptr) {
    color = shape->material.pattern->colorAt(shape, position);
  } else {
    color = shape->material.color;
  }
  auto effectiveColor = color * light.intensity;
  auto lightVector = (light.position - position).normalize();
  ambient = effectiveColor * shape->material.ambient;
  if (inShadow) {
    return ambient;
  }
  auto lightDotNormal = lightVector.dot(normalVector);
  if (lightDotNormal < 0.0) {
    diffuse = Color(0.0, 0.0, 0.0);
    specular = Color(0.0, 0.0, 0.0);
  } else {
    diffuse = effectiveColor * shape->material.diffuse * lightDotNormal;
    auto reflectVector = (-lightVector).reflect(normalVector);
    auto reflectDotEye = reflectVector.dot(eyeVector);
    if (reflectDotEye <= 0.0) {
      specular = Color(0.0, 0.0, 0.0);
    } else {
      auto factor = pow(reflectDotEye, shape->material.shininess);
      specular = light.intensity * shape->material.specular * factor;
    }
  }
  return ambient + diffuse + specular;
}
RayTracerChallenge::World::World() = default;
bool RayTracerChallenge::World::isEmpty() const { return this->objects.empty(); }
void RayTracerChallenge::World::add(const std::shared_ptr<Shape> &object) {
  this->objects.push_back(object);
}
RayTracerChallenge::World RayTracerChallenge::World::defaultWorld() {
  World world;
  world.light = PointLight(Tuple::point(-10.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));
  auto sphere1 = RayTracerChallenge::Sphere::create();
  sphere1->material.color = Color(0.8, 1.0, 0.6);
  sphere1->material.diffuse = 0.7;
  sphere1->material.specular = 0.2;
  auto sphere2 = RayTracerChallenge::Sphere::create();
  sphere2->transform = Matrix::scaling(0.5, 0.5, 0.5);
  world.add(sphere1);
  world.add(sphere2);
  return world;
}
RayTracerChallenge::Intersections RayTracerChallenge::World::intersect(Ray ray) {
  Intersections intersections;
  for (const auto &object : this->objects) {
    intersections.addAll(object->intersect(ray));
  }
  intersections.sort();
  return intersections;
}
RayTracerChallenge::Color RayTracerChallenge::World::shadeHit(const Computations &computations,
                                                              int remaining) {
  bool shadowed = isShadowed(computations.overPoint);
  auto surface = lighting(computations.object, this->light.value(), computations.overPoint,
                          computations.eyeVector, computations.normalVector, shadowed);
  auto reflected = this->reflectedColorAt(computations, remaining);
  auto refracted = this->refractedColorAt(computations, remaining);
  auto material = computations.object->material;
  if (material.reflective > 0.0 && material.transparency > 0.0) {
    auto reflectance = RayTracerChallenge::Computations::schlick(computations);
    return surface + reflected * reflectance + refracted * (1 - reflectance);
  }
  return surface + reflected + refracted;
}
RayTracerChallenge::Color RayTracerChallenge::World::colorAt(Ray ray, int remaining) {
  Intersections intersections = this->intersect(ray);
  std::optional<Intersection> hit = intersections.hit();
  if (!hit.has_value()) {
    return {0.0, 0.0, 0.0};
  }
  return shadeHit(hit.value().prepareComputations(ray, intersections), remaining);
}
RayTracerChallenge::Color RayTracerChallenge::World::reflectedColorAt(
    const Computations &computations, int remaining) {
  if (computations.object->material.reflective == 0.0 || remaining == 0) {
    return Color::BLACK;
  }
  auto reflectRay = Ray(computations.overPoint, computations.reflectionVector);
  return this->colorAt(reflectRay, remaining - 1) * computations.object->material.reflective;
}
RayTracerChallenge::Color RayTracerChallenge::World::refractedColorAt(
    const Computations &computations, int remaining) {
  if (computations.object->material.transparency == 0.0 || remaining == 0) {
    return Color::BLACK;
  }
  auto nRatio = computations.n1 / computations.n2;
  auto cosI = computations.eyeVector.dot(computations.normalVector);
  auto sin2T = pow(nRatio, 2) * (1 - pow(cosI, 2));
  if (sin2T > 1.0) {
    return Color::BLACK;
  }
  auto cosT = sqrt(1.0 - sin2T);
  auto direction
      = computations.normalVector * (nRatio * cosI - cosT) - computations.eyeVector * nRatio;
  auto refractRay = Ray(computations.underPoint, direction);
  auto color = colorAt(refractRay, remaining - 1) * computations.object->material.transparency;
  return color;
}
bool RayTracerChallenge::World::isShadowed(RayTracerChallenge::Tuple point) {
  auto distance = (light->position - point).magnitude();
  auto direction = (light->position - point).normalize();
  auto ray = RayTracerChallenge::Ray(point, direction);
  auto hit = intersect(ray).hit();
  if (hit.has_value() && hit.value().t < distance && hit.value().object->material.castShadow) {
    return true;
  }
  return false;
}
RayTracerChallenge::Camera::Camera(int hSize, int vSize, double fieldOfView) {
  this->hSize = hSize;
  this->vSize = vSize;
  this->fieldOfView = fieldOfView;
  this->transform = Matrix::identity(4);
  auto halfView = tan(this->fieldOfView / 2.0);
  auto aspect = double(this->hSize) / double(this->vSize);
  if (aspect >= 1) {
    halfWidth = halfView;
    halfHeight = halfView / aspect;
  } else {
    halfWidth = halfView * aspect;
    halfHeight = halfView;
  }
  pixelSize = (halfWidth * 2.0) / double(hSize);
}
RayTracerChallenge::Ray RayTracerChallenge::Camera::rayForPixel(int x, int y) const {
  auto xOffset = (x + 0.5) * pixelSize;
  auto yOffset = (y + 0.5) * pixelSize;
  auto worldX = double(halfWidth - xOffset);
  auto worldY = double(halfHeight - yOffset);
  auto pixel = transform.inverse() * Tuple::point(worldX, worldY, -1.0);
  auto origin = transform.inverse() * Tuple::point(0.0, 0.0, 0.0);
  auto direction = (pixel - origin).normalize();
  return {origin, direction};
}
RayTracerChallenge::Canvas RayTracerChallenge::Camera::render(World world) const {
  auto image = Canvas(hSize, vSize);
  parallelFor(vSize, [this, &world, &image](int s, int e) {
    for (int y = s; y < e; y++) {
      for (int x = 0; x < hSize; x++) {
        auto ray = rayForPixel(x, y);
        auto color = world.colorAt(ray, 4);
        image.writePixel(x, y, color);
      }
    }
  });
  return image;
}
RayTracerChallenge::StripePattern::StripePattern(RayTracerChallenge::Color a,
                                                 RayTracerChallenge::Color b) {
  this->a = a;
  this->b = b;
}
RayTracerChallenge::Color RayTracerChallenge::StripePattern::colorAt(
    std::shared_ptr<Shape> shape, RayTracerChallenge::Tuple point) const {
  auto objectPoint = shape->worldToObject(point);
  auto patternPoint = this->transform.inverse() * objectPoint;
  if (int(floor(patternPoint.x)) % 2 == 0) {
    return this->a;
  }
  return this->b;
}
RayTracerChallenge::GradientPattern::GradientPattern(RayTracerChallenge::Color a,
                                                     RayTracerChallenge::Color b) {
  this->a = a;
  this->b = b;
}
RayTracerChallenge::Color RayTracerChallenge::GradientPattern::colorAt(
    std::shared_ptr<Shape> shape, RayTracerChallenge::Tuple point) const {
  auto objectPoint = shape->transform.inverse() * point;
  auto patternPoint = this->transform.inverse() * objectPoint;
  auto distance = this->b - this->a;
  auto fraction = patternPoint.x - floor(patternPoint.x);
  return this->a + distance * fraction;
}
RayTracerChallenge::RingPattern::RingPattern(RayTracerChallenge::Color a,
                                             RayTracerChallenge::Color b) {
  this->a = a;
  this->b = b;
}
RayTracerChallenge::Color RayTracerChallenge::RingPattern::colorAt(
    std::shared_ptr<Shape> shape, RayTracerChallenge::Tuple point) const {
  auto objectPoint = shape->transform.inverse() * point;
  auto patternPoint = this->transform.inverse() * objectPoint;
  auto xSquared = pow(patternPoint.x, 2);
  auto zSquared = pow(patternPoint.z, 2);
  if (int(floor(sqrt(xSquared + zSquared))) % 2 == 0) {
    return this->a;
  }
  return this->b;
}
RayTracerChallenge::CheckersPattern::CheckersPattern(RayTracerChallenge::Color a,
                                                     RayTracerChallenge::Color b) {
  this->a = a;
  this->b = b;
}
RayTracerChallenge::Color RayTracerChallenge::CheckersPattern::colorAt(
    std::shared_ptr<Shape> shape, RayTracerChallenge::Tuple point) const {
  auto objectPoint = shape->transform.inverse() * point;
  auto patternPoint = this->transform.inverse() * objectPoint;
  auto vectorSum = int(floor(patternPoint.x) + floor(patternPoint.y) + floor(patternPoint.z));
  if (vectorSum % 2 == 0) {
    return this->a;
  }
  return this->b;
}
double RayTracerChallenge::Computations::schlick(
    const RayTracerChallenge::Computations &computations) {
  auto cos = computations.eyeVector.dot(computations.normalVector);
  if (computations.n1 > computations.n2) {
    auto n = computations.n1 / computations.n2;
    auto sin2T = pow(n, 2.0) * (1.0 - pow(cos, 2.0));
    if (sin2T > 1.0) {
      return 1.0;
    }
    auto cosT = sqrt(1.0 - sin2T);
    cos = cosT;
  }
  auto r0 = pow(((computations.n1 - computations.n2) / (computations.n1 + computations.n2)), 2.0);
  return r0 + (1 - r0) * pow((1 - cos), 5.0);
}
RayTracerChallenge::Tuple RayTracerChallenge::Cylinder::localNormalAt(Tuple point,
                                                                      Intersection hit) {
  (void)hit;
  auto dist = pow(point.x, 2.0) + pow(point.z, 2.0);
  if (dist < 1.0 && point.y >= this->maximum - EPS) {
    return {0.0, 1.0, 0.0, 0.0};
  } else if (dist < 1.0 && point.y <= this->minimum + EPS) {
    return {0.0, -1.0, 0.0, 0.0};
  }
  return {point.x, 0.0, point.z, 0.0};
}
bool checkCap(RayTracerChallenge::Ray ray, double t, double capRadius = 1.0) {
  auto x = ray.origin.x + t * ray.direction.x;
  auto z = ray.origin.z + t * ray.direction.z;
  return (pow(x, 2) + pow(z, 2)) <= capRadius;
}
void intersectCaps(RayTracerChallenge::Cylinder &cyl, RayTracerChallenge::Ray ray,
                   RayTracerChallenge::Intersections &xs) {
  if (!cyl.closed || abs(ray.direction.y) < EPS) {
    return;
  }
  auto t = (cyl.minimum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t)) {
    xs.intersections.emplace_back(t, cyl.sharedPtr);
  }
  t = (cyl.maximum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t)) {
    xs.intersections.emplace_back(t, cyl.sharedPtr);
  }
}
RayTracerChallenge::Intersections RayTracerChallenge::Cylinder::localIntersect(
    RayTracerChallenge::Ray ray) {
  auto a = pow(ray.direction.x, 2) + pow(ray.direction.z, 2);
  auto b = 2 * ray.origin.x * ray.direction.x + 2 * ray.origin.z * ray.direction.z;
  auto c = pow(ray.origin.x, 2) + pow(ray.origin.z, 2) - 1.0;
  auto disc = pow(b, 2) - 4.0 * a * c;
  if (disc < 0) {
    return {};
  }
  auto t0 = (-b - sqrt(disc)) / (2.0 * a);
  auto t1 = (-b + sqrt(disc)) / (2.0 * a);
  if (t0 > t1) {
    std::swap(t0, t1);
  }
  auto xs = Intersections();
  auto y0 = ray.origin.y + t0 * ray.direction.y;
  if (this->minimum < y0 && y0 < this->maximum) {
    xs.intersections.emplace_back(t0, this->sharedPtr);
  }
  auto y1 = ray.origin.y + t1 * ray.direction.y;
  if (this->minimum < y1 && y1 < this->maximum) {
    xs.intersections.emplace_back(t1, this->sharedPtr);
  }
  intersectCaps(*this, ray, xs);
  return xs;
}
RayTracerChallenge::BoundingBox RayTracerChallenge::Cylinder::bounds() {
  if (this->closed) {
    return {{-1.0, this->minimum, -1.0, 1.0}, {1.0, this->maximum, 1.0, 1.0}};
  }
  return {{-1.0, NEGATIVE_INFINITY, -1.0, 1.0}, {1.0, INFINITY, 1.0, 1.0}};
}
RayTracerChallenge::Tuple RayTracerChallenge::Cone::localNormalAt(Tuple point, Intersection hit) {
  (void)hit;
  auto dist = pow(point.x, 2) + pow(point.x, 2);
  if (dist < 1.0 && point.y >= this->maximum - EPS) {
    return {0.0, 1.0, 0.0, 0.0};
  } else if (dist < 1.0 && point.y <= this->minimum + EPS) {
    return {0.0, -1.0, 0.0, 0.0};
  }
  auto y = sqrt(pow(point.x, 2.0) + pow(point.z, 2.0));
  if (point.y > 0.0) {
    y = -y;
  }
  return {point.x, y, point.z, 0.0};
}
RayTracerChallenge::Intersections RayTracerChallenge::Cone::localIntersect(
    RayTracerChallenge::Ray ray) {
  auto xs = Intersections();
  auto a = pow(ray.direction.x, 2) - pow(ray.direction.y, 2) + pow(ray.direction.z, 2);
  auto b = 2 * (ray.origin.x * ray.direction.x) - 2 * (ray.origin.y * ray.direction.y)
           + 2 * (ray.origin.z * ray.direction.z);
  auto c = pow(ray.origin.x, 2) - pow(ray.origin.y, 2) + pow(ray.origin.z, 2);
  if (abs(a) < EPS && abs(b) > EPS) {
    auto t = -c / (2 * b);
    xs.intersections.emplace_back(Intersection(t, this->sharedPtr));
  }
  if (abs(a) > EPS) {
    auto disc = pow(b, 2) - 4.0 * a * c;
    if (disc < 0) {
      return {};
    }
    auto t0 = (-b - sqrt(disc)) / (2.0 * a);
    auto t1 = (-b + sqrt(disc)) / (2.0 * a);
    if (t0 > t1) {
      std::swap(t1, t0);
    }
    auto y0 = ray.origin.y + t0 * ray.direction.y;
    if (this->minimum < y0 && y0 < this->maximum) {
      xs.intersections.emplace_back(t0, this->sharedPtr);
    }
    auto y1 = ray.origin.y + t1 * ray.direction.y;
    if (this->minimum < y1 && y1 < this->maximum) {
      xs.intersections.emplace_back(t1, this->sharedPtr);
    }
  }
  if (!this->closed || abs(ray.direction.y - 0) < EPS) {
    return xs;
  }
  auto t = (this->minimum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t, abs(minimum))) {
    xs.intersections.emplace_back(t, this->sharedPtr);
  }
  t = (this->maximum - ray.origin.y) / ray.direction.y;
  if (checkCap(ray, t, abs(maximum))) {
    xs.intersections.emplace_back(t, this->sharedPtr);
  }
  return xs;
}
RayTracerChallenge::BoundingBox RayTracerChallenge::Cone::bounds() {
  if (closed) {
    auto a = abs(this->minimum);
    auto b = abs(this->maximum);
    auto limit = fmax(a, b);
    return {
        {-limit, this->minimum, -limit, 1.0},
        {limit, this->maximum, limit, 1.0},
    };
  }
  return {{NEGATIVE_INFINITY, NEGATIVE_INFINITY, NEGATIVE_INFINITY, 1.0},
          {INFINITY, INFINITY, INFINITY, 1.0}};
}
void RayTracerChallenge::Group::add(const std::shared_ptr<Shape> &object) {
  object->parent = this->sharedPtr;
  this->objects.push_back(object);
}
RayTracerChallenge::Tuple RayTracerChallenge::Group::localNormalAt(Tuple point, Intersection hit) {
  (void)hit;
  (void)point;
  return {};
}
RayTracerChallenge::Intersections RayTracerChallenge::Group::localIntersect(
    RayTracerChallenge::Ray ray) {
  auto xs = RayTracerChallenge::Intersections();
  if (!this->bounds().intersects(ray)) {
    return xs;
  }
  for (auto &object : this->objects) {
    xs.addAll(object->intersect(ray));
  }
  xs.sort();
  return xs;
}
RayTracerChallenge::BoundingBox RayTracerChallenge::Group::bounds() {
  auto box = RayTracerChallenge::BoundingBox();
  for (auto &object : this->objects) {
    auto cbox = object->parentSpaceBounds();
    box.add(cbox);
  }
  return box;
}
bool RayTracerChallenge::Group::includes(const RayTracerChallenge::Shape &object) const {
  return std::any_of(this->objects.cbegin(), this->objects.cend(),
                     [&object](auto target) { return target->includes(object); });
}
RayTracerChallenge::Tuple RayTracerChallenge::Shape::worldToObject(Tuple point) const {
  RayTracerChallenge::Tuple p = point;
  if (this->parent != nullptr) {
    p = this->parent->worldToObject(p);
  }
  return this->transform.inverse() * p;
}
RayTracerChallenge::Tuple RayTracerChallenge::Shape::normalToWorld(Tuple normal) const {
  normal = this->transform.inverse().transposed() * normal;
  normal.w = 0.0;
  normal = normal.normalize();
  if (this->parent != nullptr) {
    normal = this->parent->normalToWorld(normal);
  }
  return normal;
}
RayTracerChallenge::BoundingBox RayTracerChallenge::Shape::parentSpaceBounds() {
  return this->bounds().transform(this->transform);
}
void RayTracerChallenge::BoundingBox::add(RayTracerChallenge::Tuple point) {
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
void RayTracerChallenge::BoundingBox::add(RayTracerChallenge::BoundingBox box) {
  this->add(box.min);
  this->add(box.max);
}
bool RayTracerChallenge::BoundingBox::contains(Tuple point) const {
  return point.x >= this->min.x && point.x <= this->max.x && point.y >= this->min.y
         && point.y <= this->max.y && point.z >= this->min.z && point.z <= this->max.z;
}
bool RayTracerChallenge::BoundingBox::contains(RayTracerChallenge::BoundingBox box) const {
  return this->contains(box.max) && this->contains(box.min);
}
RayTracerChallenge::BoundingBox RayTracerChallenge::BoundingBox::transform(
    const Matrix &matrix) const {
  auto p1 = this->min;
  auto p2 = RayTracerChallenge::Tuple(this->min.x, this->min.y, this->max.z, 1.0);
  auto p3 = RayTracerChallenge::Tuple(this->min.x, this->max.y, this->min.z, 1.0);
  auto p4 = RayTracerChallenge::Tuple(this->min.x, this->max.y, this->max.z, 1.0);
  auto p5 = RayTracerChallenge::Tuple(this->max.x, this->min.y, this->min.z, 1.0);
  auto p6 = RayTracerChallenge::Tuple(this->max.x, this->min.y, this->max.z, 1.0);
  auto p7 = RayTracerChallenge::Tuple(this->min.x, this->min.y, this->max.z, 1.0);
  auto p8 = RayTracerChallenge::Tuple(this->max.x, this->max.y, this->min.z, 1.0);
  auto newBox = RayTracerChallenge::BoundingBox();
  for (Tuple p : {p1, p2, p3, p4, p5, p6, p7, p8}) {
    newBox.add(matrix * p);
  }
  return newBox;
}
bool RayTracerChallenge::BoundingBox::intersects(RayTracerChallenge::Ray ray) const {
  auto xMinMax = checkAxis(ray.origin.x, ray.direction.x, this->min.x, this->max.x);
  auto yMinMax = checkAxis(ray.origin.y, ray.direction.y, this->min.y, this->max.y);
  auto zMinMax = checkAxis(ray.origin.z, ray.direction.z, this->min.z, this->max.z);
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
RayTracerChallenge::Tuple RayTracerChallenge::Triangle::localNormalAt(Tuple point,
                                                                      Intersection hit) {
  (void)point;
  (void)hit;
  return this->normal;
}
RayTracerChallenge::Intersections RayTracerChallenge::Triangle::localIntersect(
    RayTracerChallenge::Ray ray) {
  auto dirCrossE2 = ray.direction.cross(this->e2);
  auto det = this->e1.dot(dirCrossE2);
  if (abs(det) < EPS) {
    return {};
  }
  auto f = 1.0 / det;
  auto p1ToOrigin = ray.origin - this->p1;
  auto u = f * p1ToOrigin.dot(dirCrossE2);
  if (u < 0.0 || u > 1.0) {
    return {};
  }
  auto originCrossE1 = p1ToOrigin.cross(this->e1);
  auto v = f * ray.direction.dot(originCrossE1);
  if (v < 0.0 || (u + v) > 1.0) {
    return {};
  }
  auto t = f * this->e2.dot(originCrossE1);
  auto i = RayTracerChallenge::Intersection(t, this->sharedPtr);
  i.u = u;
  i.v = v;
  return RayTracerChallenge::Intersections({i});
}
RayTracerChallenge::BoundingBox RayTracerChallenge::Triangle::bounds() {
  auto b = RayTracerChallenge::BoundingBox();
  b.add(this->p1);
  b.add(this->p2);
  b.add(this->p3);
  return b;
}
std::vector<std::shared_ptr<RayTracerChallenge::Shape>> fanTriangulation(
    std::vector<RayTracerChallenge::Tuple> vertices) {
  auto triangles = std::vector<std::shared_ptr<RayTracerChallenge::Shape>>();
  for (int index = 2; index < int(vertices.size()) - 1; ++index) {
    auto tri
        = RayTracerChallenge::Triangle::create(vertices[1], vertices[index], vertices[index + 1]);
    triangles.push_back(tri);
  }
  return triangles;
}
std::vector<std::shared_ptr<RayTracerChallenge::Shape>> fanTriangulation(
    std::vector<RayTracerChallenge::Tuple> vertices,
    std::vector<RayTracerChallenge::Tuple> normals) {
  auto triangles = std::vector<std::shared_ptr<RayTracerChallenge::Shape>>();
  for (int index = 2; index < int(vertices.size()) - 1; ++index) {
    auto tri = RayTracerChallenge::SmoothTriangle::create(vertices[1], vertices[index],
                                                          vertices[index + 1], normals[1],
                                                          normals[index], normals[index + 1]);
    triangles.push_back(tri);
  }
  return triangles;
}
ObjParser ObjParser::parse(std::stringstream &lines) {
  auto parser = ObjParser();
  parser.vertices.emplace_back();
  parser.normals.emplace_back();
  std::string oneLine;
  std::smatch subMatch;
  std::regex vertexLinePattern
      = std::regex(R"((v) (-*\d(\.*\d+)*) (-*\d(\.*\d+)*) (-*\d(\.*\d+)*))");
  std::regex vertexNormalLinePattern
      = std::regex(R"((vn) (-*\d(\.*\d+)*) (-*\d(\.*\d+)*) (-*\d(\.*\d+)*))");
  std::regex faceLinePattern = std::regex(R"((f) ((\d *){3,}))");
  std::regex faceLinePatternWithNormals = std::regex(R"((f) (\d+\/\d*\/\d+ *){3,})");
  std::regex groupLinePattern = std::regex(R"((g) ([a-zA-Z0-9]+))");
  std::string lastMentionedGroup;
  while (getline(lines, oneLine)) {
    if (std::regex_match(oneLine, subMatch, groupLinePattern)) {
      lastMentionedGroup = subMatch.str(2);
    } else if (std::regex_match(oneLine, subMatch, vertexLinePattern)) {
      auto x = std::stod(subMatch.str(2));
      auto y = std::stod(subMatch.str(4));
      auto z = std::stod(subMatch.str(6));
      parser.vertices.push_back(RayTracerChallenge::Tuple::point(x, y, z));
    } else if (std::regex_match(oneLine, subMatch, vertexNormalLinePattern)) {
      auto n1 = std::stod(subMatch.str(2));
      auto n2 = std::stod(subMatch.str(4));
      auto n3 = std::stod(subMatch.str(6));
      parser.normals.push_back(RayTracerChallenge::Tuple::vector(n1, n2, n3));
    } else if (std::regex_match(oneLine, subMatch, faceLinePattern)) {
      auto indices = subMatch.str(2);
      std::stringstream ss(indices);
      std::string s;
      std::vector<RayTracerChallenge::Tuple> targetVertices;
      targetVertices.emplace_back();
      while (std::getline(ss, s, ' ')) {
        targetVertices.push_back(parser.vertices[std::stoi(s)]);
      }
      auto triangles = fanTriangulation(targetVertices);
      for (const auto &triangle : triangles) {
        if (lastMentionedGroup.empty()) {
          std::dynamic_pointer_cast<RayTracerChallenge::Group>(parser.defaultGroup)->add(triangle);
        } else {
          if (!parser.groups.count(lastMentionedGroup)) {
            parser.groups[lastMentionedGroup] = RayTracerChallenge::Group::create();
          }
          std::dynamic_pointer_cast<RayTracerChallenge::Group>(parser.groups[lastMentionedGroup])
              ->add(triangle);
        }
      }
    } else if (std::regex_match(oneLine, faceLinePatternWithNormals)) {
      std::stringstream ss(oneLine);
      std::string s;
      std::vector<RayTracerChallenge::Tuple> targetVertices;
      std::vector<RayTracerChallenge::Tuple> targetNormals;
      targetVertices.emplace_back();
      targetNormals.emplace_back();
      while (std::getline(ss, s, ' ')) {
        if (s == "f") {
          continue;
        }
        auto firstSlashIndex = s.find('/');
        auto lastSlashIndex = s.rfind('/');
        targetVertices.push_back(parser.vertices[std::stoi(s.substr(0, firstSlashIndex))]);
        targetNormals.push_back(
            parser.normals[stoi(s.substr(lastSlashIndex + 1, s.size() - lastSlashIndex - 1))]);
      }
      auto triangles = fanTriangulation(targetVertices, targetNormals);
      for (const auto &triangle : triangles) {
        if (lastMentionedGroup.empty()) {
          std::dynamic_pointer_cast<RayTracerChallenge::Group>(parser.defaultGroup)->add(triangle);
        } else {
          if (!parser.groups.count(lastMentionedGroup)) {
            parser.groups[lastMentionedGroup] = RayTracerChallenge::Group::create();
          }
          std::dynamic_pointer_cast<RayTracerChallenge::Group>(parser.groups[lastMentionedGroup])
              ->add(triangle);
        }
      }
    } else {
      std::cout << "Skipped line: " << oneLine << std::endl;
    }
  }
  return parser;
}
std::shared_ptr<RayTracerChallenge::Shape> ObjParser::getObjects() {
  auto group = RayTracerChallenge::Group::create();
  for (auto const &item : this->groups) {
    std::dynamic_pointer_cast<RayTracerChallenge::Group>(group)->add(item.second);
  }
  return group;
}
RayTracerChallenge::Tuple RayTracerChallenge::SmoothTriangle::localNormalAt(Tuple point,
                                                                            Intersection hit) {
  (void)point;
  return this->n2 * hit.u + this->n3 * hit.v + this->n1 * (1 - hit.u - hit.v);
}
RayTracerChallenge::BoundingBox RayTracerChallenge::SmoothTriangle::bounds() {
  auto b = RayTracerChallenge::BoundingBox();
  b.add(this->p1);
  b.add(this->p2);
  b.add(this->p3);
  return b;
}
RayTracerChallenge::Tuple RayTracerChallenge::CSG::localNormalAt(
    RayTracerChallenge::Tuple point, RayTracerChallenge::Intersection hit) {
  (void)point;
  (void)hit;
  return {};
}
RayTracerChallenge::BoundingBox RayTracerChallenge::CSG::bounds() {
  auto box = RayTracerChallenge::BoundingBox();
  box.add(this->left->parentSpaceBounds());
  box.add(this->right->parentSpaceBounds());
  return box;
}
RayTracerChallenge::Intersections RayTracerChallenge::CSG::localIntersect(
    RayTracerChallenge::Ray ray) {
  auto xs = Intersections();
  if (!this->bounds().intersects(ray)) {
    return xs;
  }
  xs.addAll(this->left->intersect(ray));
  xs.addAll(this->right->intersect(ray));
  xs.sort();
  return filterIntersections(xs);
}
bool RayTracerChallenge::CSG::intersectionAllowed(RayTracerChallenge::CSG::Operation op,
                                                  bool leftHit, bool inLeft, bool inRight) {
  switch (op) {
    case Union:
      return (leftHit && !inRight) || (!leftHit && !inLeft);
      break;
    case Intersection:
      return (leftHit && inRight) || (!leftHit && inLeft);
      break;
    case Difference:
      return (leftHit && !inRight) || (!leftHit && inLeft);
      break;
    default:
      return false;
  }
}
RayTracerChallenge::Intersections RayTracerChallenge::CSG::filterIntersections(
    const Intersections &intersections) const {
  bool inLeft = false;
  bool inRight = false;
  auto result = RayTracerChallenge::Intersections();
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
bool RayTracerChallenge::CSG::includes(const RayTracerChallenge::Shape &object) const {
  return this->left->includes(object) || this->right->includes(object);
}
