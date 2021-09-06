#define _USE_MATH_DEFINES
#include <fmt/format.h>
#include <raytracerchallenge/raytracerchallenge.h>

#include <chrono>
#include <cmath>
#include <utility>

using namespace raytracerchallenge;

const double EPS = 0.0001;

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
bool RayTracerChallenge::Tuple::doubleEquals(double x, double y) { return abs(x - y) < EPS; }

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

RayTracerChallenge::Matrix::Row::Row(std::vector<double> r) : row(std::move(r)) {}
double RayTracerChallenge::Matrix::Row::operator[](unsigned int y) { return row.at(y); }
RayTracerChallenge::Matrix::Matrix(unsigned int x, unsigned int y,
                                   std::vector<std::vector<double>> m) {
  this->m.resize(x, std::vector<double>(y, false));
  this->m = std::move(m);
}
RayTracerChallenge::Matrix::Matrix() = default;
RayTracerChallenge::Matrix::Row RayTracerChallenge::Matrix::operator[](const unsigned int x) const {
  return RayTracerChallenge::Matrix::Row(m.at(x));
}
bool RayTracerChallenge::Matrix::doubleEquals(double x, double y) { return abs(x - y) < EPS; }
bool RayTracerChallenge::Matrix::operator==(const Matrix &matrix) const {
  for (int x = 0; x < (int)this->m.size(); x++) {
    for (int y = 0; y < (int)this->m[0].size(); y++) {
      if (!doubleEquals(this->m[x][y], matrix.m[x][y])) {
        return false;
      }
    }
  }
  return true;
}
bool RayTracerChallenge::Matrix::operator!=(const Matrix &matrix) const {
  return this->m != matrix.m;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::operator*(
    const RayTracerChallenge::Matrix &matrix) const {
  std::vector<std::vector<double>> res(this->m.size());
  for (int x = 0; x < (int)this->m.size(); x++) {
    res[x] = std::vector<double>(this->m[0].size());
    for (int y = 0; y < (int)this->m[0].size(); y++) {
      double cellVal = double();
      for (int z = 0; z < (int)this->m.size(); z++) {
        cellVal += this->m[x][z] * matrix.m[z][y];
      }
      res[x][y] = cellVal;
    }
  }
  return {static_cast<unsigned int>(res.size()), static_cast<unsigned int>(res[0].size()), res};
}
RayTracerChallenge::Tuple RayTracerChallenge::Matrix::operator*(
    const RayTracerChallenge::Tuple &tuple) const {
  Tuple res = Tuple(0, 0, 0, 0);
  res.x = Tuple(this->m[0][0], this->m[0][1], this->m[0][2], this->m[0][3]).dot(tuple);
  res.y = Tuple(this->m[1][0], this->m[1][1], this->m[1][2], this->m[1][3]).dot(tuple);
  res.z = Tuple(this->m[2][0], this->m[2][1], this->m[2][2], this->m[2][3]).dot(tuple);
  res.w = Tuple(this->m[3][0], this->m[3][1], this->m[3][2], this->m[3][3]).dot(tuple);
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::identity(unsigned int size) {
  std::vector<std::vector<double>> res(size);
  unsigned int diagX = 0;
  unsigned int diagY = 0;
  for (unsigned int x = 0; x < size; x++) {
    res[x] = std::vector<double>(size);
    for (unsigned int y = 0; y < size; y++) {
      if (x == diagX && y == diagY) {
        res[x][y] = 1.0;
        diagX++;
        diagY++;
      } else {
        res[x][y] = 0.0;
      }
    }
  }
  return {size, size, res};
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::transposed() {
  std::vector<std::vector<double>> res(this->m.size());
  for (unsigned int x = 0; x < this->m.size(); x++) {
    res[x] = std::vector<double>(this->m.size());
  }
  for (unsigned int x = 0; x < this->m.size(); x++) {
    for (unsigned int y = 0; y < this->m.size(); y++) {
      res[y][x] = this->m[x][y];
    }
  }
  return {static_cast<unsigned int>(this->m.size()), static_cast<unsigned int>(this->m.size()),
          res};
}

double RayTracerChallenge::Matrix::determinant() {
  if (this->m.size() == 2) {
    return this->m[0][0] * this->m[1][1] - this->m[0][1] * this->m[1][0];
  }
  double det = 0;
  for (int col = 0; col < (int)m.size(); col++) {
    det += m[0][col] * cofactor(0, col);
  }
  return det;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::submatrix(unsigned int x, unsigned int y) {
  std::vector<std::vector<double>> res(this->m.size());
  res = m;
  for (auto &re : res) {
    re.erase(re.cbegin() + y);
  }
  res.erase(res.cbegin() + x);
  return {static_cast<unsigned int>(res.size()), static_cast<unsigned int>(res.size()), res};
}
double RayTracerChallenge::Matrix::minor(unsigned int x, unsigned int y) {
  return this->submatrix(x, y).determinant();
}
double RayTracerChallenge::Matrix::cofactor(unsigned int x, unsigned int y) {
  double det = this->submatrix(x, y).determinant();
  if ((x + y) % 2 == 0) {
    return det;
  }
  return -det;
}
bool RayTracerChallenge::Matrix::invertible() { return determinant() != 0.0; }
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::inverse() {
  std::vector<std::vector<double>> res(this->m.size());
  for (unsigned int x = 0; x < this->m.size(); x++) {
    res[x] = std::vector<double>(this->m.size());
  }
  for (unsigned int row = 0; row < this->m.size(); row++) {
    for (unsigned int col = 0; col < this->m.size(); col++) {
      res[col][row] = this->cofactor(row, col) / determinant();
    }
  }
  return {static_cast<unsigned int>(res.size()), static_cast<unsigned int>(res.size()), res};
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::translation(double x, double y, double z) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[0][3] = x;
  res.m[1][3] = y;
  res.m[2][3] = z;
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::scaling(double x, double y, double z) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[0][0] = x;
  res.m[1][1] = y;
  res.m[2][2] = z;
  res.m[3][3] = 1;
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotationX(double radians) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[1][1] = cos(radians);
  res.m[1][2] = -sin(radians);
  res.m[2][1] = sin(radians);
  res.m[2][2] = cos(radians);
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotationY(double radians) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[0][0] = cos(radians);
  res.m[0][2] = sin(radians);
  res.m[2][0] = -sin(radians);
  res.m[2][2] = cos(radians);
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotationZ(double radians) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[0][0] = cos(radians);
  res.m[0][1] = -sin(radians);
  res.m[1][0] = sin(radians);
  res.m[1][1] = cos(radians);
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::shearing(double xy, double xz, double yx,
                                                                double yz, double zx, double zy) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[0][1] = xy;
  res.m[0][2] = xz;
  res.m[1][0] = yx;
  res.m[1][2] = yz;
  res.m[2][0] = zx;
  res.m[2][1] = zy;
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
}
RayTracerChallenge::Intersections RayTracerChallenge::Shape::intersect(
    RayTracerChallenge::Ray ray) {
  RayTracerChallenge::Ray transformed = ray.transform(this->transform.inverse());
  return localIntersect(transformed);
}
RayTracerChallenge::Intersections RayTracerChallenge::Shape::localIntersect(Ray ray) {
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
      RayTracerChallenge::Intersection(t1, *this), RayTracerChallenge::Intersection(t2, *this)});
}
bool RayTracerChallenge::Shape::operator==(const Shape &object) const {
  return this->transform == object.transform && this->material == object.material;
}
bool RayTracerChallenge::Shape::is(const Shape &object) const { return this->id == object.id; }
RayTracerChallenge::Intersection::Intersection(double t, Shape object) {
  this->t = t;
  this->object = std::move(object);
}
RayTracerChallenge::Intersection::Intersection() = default;
bool RayTracerChallenge::Intersection::operator==(const Intersection &intersection) const {
  return this->object.is(intersection.object) && this->t == intersection.t;
}
bool RayTracerChallenge::Intersection::operator<(
    const RayTracerChallenge::Intersection &intersection) const {
  return this->t < intersection.t;
}
RayTracerChallenge::Computations RayTracerChallenge::Intersection::prepareComputations(
    RayTracerChallenge::Ray ray) {
  auto computations = RayTracerChallenge::Computations();
  computations.t = this->t;
  computations.object = this->object;
  computations.point = ray.position(t);
  computations.eyeVector = -ray.direction;
  computations.normalVector = this->object.normalAt(computations.point);
  if (computations.normalVector.dot(computations.eyeVector) < 0.0) {
    computations.inside = true;
    computations.normalVector = -computations.normalVector;
  }
  computations.overPoint = computations.point + (computations.normalVector * EPS);
  return computations;
}
std::optional<RayTracerChallenge::Intersection> RayTracerChallenge::Intersections::hit() {
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
RayTracerChallenge::Tuple RayTracerChallenge::Shape::localNormalAt(
    RayTracerChallenge::Tuple point) {
  return point - RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
}
RayTracerChallenge::Tuple RayTracerChallenge::Shape::normalAt(RayTracerChallenge::Tuple point) {
  auto objectPoint = this->transform.inverse() * point;
  auto objectNormal = localNormalAt(objectPoint);
  auto worldNormal = (this->transform.inverse().transposed()) * objectNormal;
  worldNormal.w = 0.0;
  return worldNormal.normalize();
}
bool RayTracerChallenge::Shape::operator<(const RayTracerChallenge::Shape &object) const {
  return this->id < object.id;
}
RayTracerChallenge::Tuple RayTracerChallenge::Plane::localNormalAt(Tuple point) {
  (void)point;
  return RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
}
RayTracerChallenge::Intersections RayTracerChallenge::Plane::localIntersect(Ray ray) {
  if (abs(ray.direction.y) < EPS) {
    return {};
  }
  auto t = -ray.origin.y / ray.direction.y;
  return Intersections({RayTracerChallenge::Intersection(t, *this)});
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
RayTracerChallenge::Color RayTracerChallenge::lighting(RayTracerChallenge::Material material,
                                                       RayTracerChallenge::PointLight light,
                                                       RayTracerChallenge::Tuple position,
                                                       RayTracerChallenge::Tuple eyeVector,
                                                       RayTracerChallenge::Tuple normalVector,
                                                       bool inShadow) {
  Color diffuse;
  Color specular;
  Color ambient;
  auto effectiveColor = material.color * light.intensity;
  auto lightVector = (light.position - position).normalize();
  ambient = effectiveColor * material.ambient;
  if (inShadow) {
    return ambient;
  }
  auto lightDotNormal = lightVector.dot(normalVector);
  if (lightDotNormal < 0.0) {
    diffuse = Color(0.0, 0.0, 0.0);
    specular = Color(0.0, 0.0, 0.0);
  } else {
    diffuse = effectiveColor * material.diffuse * lightDotNormal;
    auto reflectVector = (-lightVector).reflect(normalVector);
    auto reflectDotEye = reflectVector.dot(eyeVector);
    if (reflectDotEye <= 0.0) {
      specular = Color(0.0, 0.0, 0.0);
    } else {
      auto factor = pow(reflectDotEye, material.shininess);
      specular = light.intensity * material.specular * factor;
    }
  }
  return ambient + diffuse + specular;
}
RayTracerChallenge::World::World() = default;
bool RayTracerChallenge::World::contains(Shape &object) {
  return std::find(this->objects.begin(), this->objects.end(), object) != this->objects.end();
}
bool RayTracerChallenge::World::isEmpty() const { return this->objects.empty(); }
void RayTracerChallenge::World::add(RayTracerChallenge::Shape &object) {
  this->objects.push_back(object);
}
RayTracerChallenge::World RayTracerChallenge::World::defaultWorld() {
  World world;
  world.light = PointLight(Tuple::point(-10.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));
  Sphere sphere1;
  sphere1.material.color = Color(0.8, 1.0, 0.6);
  sphere1.material.diffuse = 0.7;
  sphere1.material.specular = 0.2;
  Sphere sphere2;
  sphere2.transform = Matrix::scaling(0.5, 0.5, 0.5);
  world.add(sphere1);
  world.add(sphere2);
  return world;
}
RayTracerChallenge::Intersections RayTracerChallenge::World::intersect(Ray ray) {
  Intersections intersections;
  for (Shape object : this->objects) {
    intersections.addAll(object.intersect(ray));
  }
  intersections.sort();
  return intersections;
}
RayTracerChallenge::Color RayTracerChallenge::World::shadeHit(const Computations &computations) {
  bool shadowed = isShadowed(computations.overPoint);
  return lighting(computations.object.material, this->light.value(), computations.overPoint,
                  computations.eyeVector, computations.normalVector, shadowed);
}
RayTracerChallenge::Color RayTracerChallenge::World::colorAt(Ray ray) {
  Intersections intersections = this->intersect(ray);
  std::optional<Intersection> hit = intersections.hit();
  if (!hit.has_value()) {
    return {0.0, 0.0, 0.0};
  }
  return shadeHit(hit.value().prepareComputations(ray));
}
bool RayTracerChallenge::World::isShadowed(RayTracerChallenge::Tuple point) {
  auto distance = (light->position - point).magnitude();
  auto direction = (light->position - point).normalize();
  auto ray = RayTracerChallenge::Ray(point, direction);
  auto hit = intersect(ray).hit();
  if (hit.has_value() && hit.value().t < distance) {
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
RayTracerChallenge::Ray RayTracerChallenge::Camera::rayForPixel(int x, int y) {
  auto xOffset = (x + 0.5) * pixelSize;
  auto yOffset = (y + 0.5) * pixelSize;
  auto worldX = double(halfWidth - xOffset);
  auto worldY = double(halfHeight - yOffset);
  auto pixel = transform.inverse() * Tuple::point(worldX, worldY, -1.0);
  auto origin = transform.inverse() * Tuple::point(0.0, 0.0, 0.0);
  auto direction = (pixel - origin).normalize();
  return {origin, direction};
}
RayTracerChallenge::Canvas RayTracerChallenge::Camera::render(World world) {
  auto image = Canvas(hSize, vSize);
  for (int y = 0; y < vSize; y++) {
    for (int x = 0; x < hSize; x++) {
      auto ray = rayForPixel(x, y);
      auto color = world.colorAt(ray);
      image.writePixel(x, y, color);
    }
  }
  return image;
}