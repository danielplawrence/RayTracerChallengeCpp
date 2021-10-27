
#include <raytracerchallenge/base/Color.h>
#include <raytracerchallenge/base/Tuple.h>

namespace raytracerchallenge {
  Color::Color() {
    this->red = 0.0;
    this->green = 0.0;
    this->blue = 0.0;
  }
  Color::Color(double red, double green, double blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
  }
  Color Color::operator*(double f) const {
    Tuple t = Tuple(this->red, this->green, this->blue, 0) * f;
    return {t.x, t.y, t.z};
  }
  Color Color::operator-(const Color &c) const {
    Tuple t1 = Tuple(this->red, this->green, this->blue, 0);
    Tuple t2 = Tuple(c.red, c.green, c.blue, 0);
    Tuple t3 = t1 - t2;
    return {t3.x, t3.y, t3.z};
  }
  Color Color::operator+(const Color &c) const {
    Tuple t1 = Tuple(this->red, this->green, this->blue, 0);
    Tuple t2 = Tuple(c.red, c.green, c.blue, 0);
    Tuple t3 = t1 + t2;
    return {t3.x, t3.y, t3.z};
  }
  Color Color::operator*(const Color &c) const {
    return {this->red * c.red, this->green * c.green, this->blue * c.blue};
  }
  bool Color::operator==(const Color &c) const {
    Tuple t1 = Tuple(this->red, this->green, this->blue, 0);
    Tuple t2 = Tuple(c.red, c.green, c.blue, 0);
    return t1 == t2;
  }
}  // namespace raytracerchallenge