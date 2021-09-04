#define _USE_MATH_DEFINES
#include <fmt/format.h>
#include <raytracerchallenge/raytracerchallenge.h>

#include <chrono>
#include <cmath>
#include <utility>

using namespace raytracerchallenge;

RayTracerChallenge::Tuple::Tuple() {
  this->x = 0.0f;
  this->y = 0.0f;
  this->z = 0.0f;
  this->w = 0.0f;
}
RayTracerChallenge::Tuple::Tuple(float x, float y, float z, float w) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}
bool RayTracerChallenge::Tuple::isVector() const { return w == 0; }
float RayTracerChallenge::Tuple::magnitude() const {
  return sqrtf(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2) + powf(this->w, 2));
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::normalize() const {
  return {this->x / this->magnitude(), this->y / this->magnitude(), this->z / this->magnitude(),
          this->w / this->magnitude()};
}
bool RayTracerChallenge::Tuple::operator==(const RayTracerChallenge::Tuple &t) const {
  return floatEquals(t.x, this->x) && floatEquals(t.y, this->y) && floatEquals(t.z, this->z)
         && floatEquals(t.w, this->w);
}
float RayTracerChallenge::Tuple::dot(const RayTracerChallenge::Tuple &t) const {
  return this->x * t.x + this->y * t.y + this->z * t.z + this->w * t.w;
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::cross(
    const RayTracerChallenge::Tuple &t) const {
  return RayTracerChallenge::Tuple::vector(
      this->y * t.z - this->z * t.y, this->z * t.x - this->x * t.z, this->x * t.y - this->y * t.x);
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::reflect(
    const RayTracerChallenge::Tuple &t) const {
  return *this - t * 2.0f * this->dot(t);
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
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator*(float f) const {
  return {f * this->x, f * this->y, f * this->z, f * this->w};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::operator/(float f) const {
  return {this->x / f, this->y / f, this->z / f, this->w / f};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::point(float x, float y, float z) {
  return {x, y, z, 1.0};
}
RayTracerChallenge::Tuple RayTracerChallenge::Tuple::vector(float x, float y, float z) {
  return {x, y, z, 0.0};
}
bool RayTracerChallenge::Tuple::floatEquals(float x, float y) { return abs(x - y) < 0.00001; }

RayTracerChallenge::Color::Color() {
  this->red = 0.0f;
  this->green = 0.0f;
  this->blue = 0.0f;
}
RayTracerChallenge::Color::Color(float red, float green, float blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
}
RayTracerChallenge::Color RayTracerChallenge::Color::operator*(float f) const {
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
      std::vector<float> colorVals{c.red, c.green, c.blue};
      std::for_each(colorVals.begin(), colorVals.end(), [&line, &header](float f) {
        std::string val = fmt::to_string(std::ceil(std::clamp(f * 255.0f, 0.0f, 255.0f)));
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

RayTracerChallenge::Matrix::Row::Row(std::vector<float> r) : row(std::move(r)) {}
float RayTracerChallenge::Matrix::Row::operator[](unsigned int y) { return row.at(y); }
RayTracerChallenge::Matrix::Matrix(unsigned int x, unsigned int y,
                                   std::vector<std::vector<float>> m) {
  this->m.resize(x, std::vector<float>(y, false));
  this->m = std::move(m);
}
RayTracerChallenge::Matrix::Matrix() = default;
RayTracerChallenge::Matrix::Row RayTracerChallenge::Matrix::operator[](const unsigned int x) const {
  return RayTracerChallenge::Matrix::Row(m.at(x));
}
bool RayTracerChallenge::Matrix::floatEquals(float x, float y) { return abs(x - y) < 0.0001; }
bool RayTracerChallenge::Matrix::operator==(const Matrix &matrix) const {
  for (int x = 0; x < (int)this->m.size(); x++) {
    for (int y = 0; y < (int)this->m[0].size(); y++) {
      if (!floatEquals(this->m[x][y], matrix.m[x][y])) {
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
  std::vector<std::vector<float>> res(this->m.size());
  for (int x = 0; x < (int)this->m.size(); x++) {
    res[x] = std::vector<float>(this->m[0].size());
    for (int y = 0; y < (int)this->m[0].size(); y++) {
      float cellVal = float();
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
  std::vector<std::vector<float>> res(size);
  unsigned int diagX = 0;
  unsigned int diagY = 0;
  for (unsigned int x = 0; x < size; x++) {
    res[x] = std::vector<float>(size);
    for (unsigned int y = 0; y < size; y++) {
      if (x == diagX && y == diagY) {
        res[x][y] = 1.0f;
        diagX++;
        diagY++;
      } else {
        res[x][y] = 0.0f;
      }
    }
  }
  return {size, size, res};
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::transposed() {
  std::vector<std::vector<float>> res(this->m.size());
  for (unsigned int x = 0; x < this->m.size(); x++) {
    res[x] = std::vector<float>(this->m.size());
  }
  for (unsigned int x = 0; x < this->m.size(); x++) {
    for (unsigned int y = 0; y < this->m.size(); y++) {
      res[y][x] = this->m[x][y];
    }
  }
  return {static_cast<unsigned int>(this->m.size()), static_cast<unsigned int>(this->m.size()),
          res};
}

float RayTracerChallenge::Matrix::determinant() {
  if (this->m.size() == 2) {
    return this->m[0][0] * this->m[1][1] - this->m[0][1] * this->m[1][0];
  }
  float det = 0;
  for (int col = 0; col < (int)m.size(); col++) {
    det += m[0][col] * cofactor(0, col);
  }
  return det;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::submatrix(unsigned int x, unsigned int y) {
  std::vector<std::vector<float>> res(this->m.size());
  res = m;
  for (auto &re : res) {
    re.erase(re.cbegin() + y);
  }
  res.erase(res.cbegin() + x);
  return {static_cast<unsigned int>(res.size()), static_cast<unsigned int>(res.size()), res};
}
float RayTracerChallenge::Matrix::minor(unsigned int x, unsigned int y) {
  return this->submatrix(x, y).determinant();
}
float RayTracerChallenge::Matrix::cofactor(unsigned int x, unsigned int y) {
  float det = this->submatrix(x, y).determinant();
  if ((x + y) % 2 == 0) {
    return det;
  }
  return -det;
}
bool RayTracerChallenge::Matrix::invertible() { return determinant() != 0.0f; }
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::inverse() {
  std::vector<std::vector<float>> res(this->m.size());
  for (unsigned int x = 0; x < this->m.size(); x++) {
    res[x] = std::vector<float>(this->m.size());
  }
  for (unsigned int row = 0; row < this->m.size(); row++) {
    for (unsigned int col = 0; col < this->m.size(); col++) {
      res[col][row] = this->cofactor(row, col) / determinant();
    }
  }
  return {static_cast<unsigned int>(res.size()), static_cast<unsigned int>(res.size()), res};
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::translation(float x, float y, float z) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[0][3] = x;
  res.m[1][3] = y;
  res.m[2][3] = z;
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::scaling(float x, float y, float z) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[0][0] = x;
  res.m[1][1] = y;
  res.m[2][2] = z;
  res.m[3][3] = 1;
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotationX(float radians) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[1][1] = cos(radians);
  res.m[1][2] = -sin(radians);
  res.m[2][1] = sin(radians);
  res.m[2][2] = cos(radians);
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotationY(float radians) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[0][0] = cos(radians);
  res.m[0][2] = sin(radians);
  res.m[2][0] = -sin(radians);
  res.m[2][2] = cos(radians);
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotationZ(float radians) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[0][0] = cos(radians);
  res.m[0][1] = -sin(radians);
  res.m[1][0] = sin(radians);
  res.m[1][1] = cos(radians);
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::shearing(float xy, float xz, float yx,
                                                                float yz, float zx, float zy) {
  RayTracerChallenge::Matrix res = RayTracerChallenge::Matrix::identity(4);
  res.m[0][1] = xy;
  res.m[0][2] = xz;
  res.m[1][0] = yx;
  res.m[1][2] = yz;
  res.m[2][0] = zx;
  res.m[2][1] = zy;
  return res;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::translated(float x, float y, float z) const {
  RayTracerChallenge::Matrix t = translation(x, y, z);
  return t * *this;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::scaled(float x, float y, float z) const {
  RayTracerChallenge::Matrix t = scaling(x, y, z);
  return t * *this;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotatedX(float radians) const {
  RayTracerChallenge::Matrix t = rotationX(radians);
  return t * *this;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotatedY(float radians) const {
  RayTracerChallenge::Matrix t = rotationY(radians);
  return t * *this;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::rotatedZ(float radians) const {
  RayTracerChallenge::Matrix t = rotationZ(radians);
  return t * *this;
}
RayTracerChallenge::Matrix RayTracerChallenge::Matrix::sheared(float xy, float xz, float yx,
                                                               float yz, float zx, float zy) const {
  RayTracerChallenge::Matrix t = shearing(xy, xz, yx, yz, zx, zy);
  return t * *this;
}
RayTracerChallenge::Ray::Ray(RayTracerChallenge::Tuple origin,
                             RayTracerChallenge::Tuple direction) {
  this->origin = origin;
  this->direction = direction;
}
RayTracerChallenge::Tuple RayTracerChallenge::Ray::position(float t) const {
  return RayTracerChallenge::Tuple(this->origin + this->direction * t);
}
RayTracerChallenge::Ray RayTracerChallenge::Ray::transform(
    const RayTracerChallenge::Matrix &matrix) {
  return {matrix * this->origin, matrix * this->direction};
}
RayTracerChallenge::Object::Object() {
  uint64_t current_time_us = std::chrono::duration_cast<std::chrono::microseconds>(
                                 std::chrono::high_resolution_clock::now().time_since_epoch())
                                 .count();
  this->id = fmt::to_string(current_time_us);
  this->transform = Matrix::identity(4);
}
RayTracerChallenge::Intersections RayTracerChallenge::Object::intersect(
    RayTracerChallenge::Ray ray) {
  RayTracerChallenge::Ray transformed = ray.transform(this->transform.inverse());
  Tuple sphereToRay = transformed.origin - RayTracerChallenge::Tuple::point(0.0f, 0.0f, 0.0f);
  float a = transformed.direction.dot(transformed.direction);
  float b = 2.0f * transformed.direction.dot(sphereToRay);
  float c = sphereToRay.dot(sphereToRay) - 1.0f;
  float discriminant = pow(b, 2.0f) - 4.0f * a * c;
  if (discriminant < 0.0f) {
    return Intersections(std::vector<RayTracerChallenge::Intersection>(0));
  }
  float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
  float t2 = (-b + sqrt(discriminant)) / (2.0f * a);

  return Intersections(std::vector<RayTracerChallenge::Intersection>{
      RayTracerChallenge::Intersection(t1, *this), RayTracerChallenge::Intersection(t2, *this)});
}
bool RayTracerChallenge::Object::operator==(const Object &object) const {
  return this->transform == object.transform && this->material == object.material;
}
bool RayTracerChallenge::Object::is(const Object &object) const { return this->id == object.id; }
RayTracerChallenge::Intersection::Intersection(float t, Object object) {
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
  return computations;
}
std::optional<RayTracerChallenge::Intersection> RayTracerChallenge::Intersections::hit() {
  std::vector<RayTracerChallenge::Intersection> nonNegative;
  std::copy_if(intersections.cbegin(), intersections.cend(), std::back_inserter(nonNegative),
               [](Intersection i) { return i.t >= 0.0f; });
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
RayTracerChallenge::Tuple RayTracerChallenge::Object::normalAt(RayTracerChallenge::Tuple point) {
  auto objectPoint = this->transform.inverse() * point;
  auto objectNormal = objectPoint - RayTracerChallenge::Tuple::point(0.0f, 0.0f, 0.0f);
  auto worldNormal = (this->transform.inverse().transposed()) * objectNormal;
  worldNormal.w = 0.0f;
  return worldNormal.normalize();
}
bool RayTracerChallenge::Object::operator<(const RayTracerChallenge::Object &object) const {
  return this->id < object.id;
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
                                                       RayTracerChallenge::Tuple normalVector) {
  Color diffuse;
  Color specular;
  Color ambient;
  auto effectiveColor = material.color * light.intensity;
  auto lightVector = (light.position - position).normalize();
  ambient = effectiveColor * material.ambient;
  auto lightDotNormal = lightVector.dot(normalVector);
  if (lightDotNormal < 0.0f) {
    diffuse = Color(0.0f, 0.0f, 0.0f);
    specular = Color(0.0f, 0.0f, 0.0f);
  } else {
    diffuse = effectiveColor * material.diffuse * lightDotNormal;
    auto reflectVector = (-lightVector).reflect(normalVector);
    auto reflectDotEye = reflectVector.dot(eyeVector);
    if (reflectDotEye <= 0.0f) {
      specular = Color(0.0f, 0.0f, 0.0f);
    } else {
      auto factor = pow(reflectDotEye, material.shininess);
      specular = light.intensity * material.specular * factor;
    }
  }
  return ambient + diffuse + specular;
}
RayTracerChallenge::World::World() = default;
bool RayTracerChallenge::World::contains(Object &object) {
  return std::find(this->objects.begin(), this->objects.end(), object) != this->objects.end();
}
bool RayTracerChallenge::World::isEmpty() { return this->objects.empty(); }
void RayTracerChallenge::World::add(RayTracerChallenge::Object &object) {
  this->objects.push_back(object);
}
RayTracerChallenge::World RayTracerChallenge::World::defaultWorld() {
  World world;
  world.light = PointLight(Tuple::point(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
  Sphere sphere1;
  sphere1.material.color = Color(0.8f, 1.0f, 0.6f);
  sphere1.material.diffuse = 0.6f;
  sphere1.material.specular = 0.2f;
  Sphere sphere2;
  sphere2.transform = Matrix::scaling(0.5f, 0.5f, 0.5f);
  world.add(sphere1);
  world.add(sphere2);
  return world;
}
RayTracerChallenge::Intersections RayTracerChallenge::World::intersect(Ray ray) {
  Intersections intersections;
  for (Object object : this->objects) {
    intersections.addAll(object.intersect(ray));
  }
  intersections.sort();
  return intersections;
}