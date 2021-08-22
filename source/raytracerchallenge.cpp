#include <fmt/format.h>
#include <raytracerchallenge/raytracerchallenge.h>

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