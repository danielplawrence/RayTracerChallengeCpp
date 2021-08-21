#include <fmt/format.h>
#include <raytracerchallenge/raytracerchallenge.h>

using namespace raytracerchallenge;

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
void RayTracerChallenge::Canvas::writePixel(int x, int y, RayTracerChallenge::Color &c) {
  this->pixels[x][y] = c;
}
RayTracerChallenge::Color RayTracerChallenge::Canvas::pixelAt(int x, int y) {
  return this->pixels[x][y];
}

std::string RayTracerChallenge::Canvas::toPortablePixmap() {
  std::string header("P3\n5 3\n255\n");
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
