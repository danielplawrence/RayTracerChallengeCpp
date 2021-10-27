#include <fmt/format.h>
#include <raytracerchallenge/base/Canvas.h>
#include <raytracerchallenge/base/Color.h>

#include <vector>

namespace raytracerchallenge {
  Canvas::Canvas(int width, int height) {
    this->width = width;
    this->height = height;
    this->pixels = std::vector<std::vector<Color>>(width);
    for (int x = 0; x < width; x++) {
      this->pixels[x] = std::vector<Color>(height);
    }
  }
  void Canvas::writePixel(int x, int y, Color c) { this->pixels[x][y] = c; }
  Color Canvas::pixelAt(int x, int y) { return this->pixels[x][y]; }
  std::string Canvas::toPortablePixmap() {
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
}  // namespace raytracerchallenge