#pragma once

#include <vector>

#include "Color.h"

namespace raytracerchallenge {
  /**
   * @brief Represents the image canvas
   */
  class Canvas {
  public:
    int width{};
    int height{};
    /**
     * @brief Create a new Canvas
     * @param width width of the Canvas in pixels
     * @param height height of the Canvas in pixels
     */
    Canvas(int width, int height);
    /**
     * @brief Write a pixel with color c to coordinates x and y
     * @param x x coordinate
     * @param y y coordinate
     * @param c color
     */
    void writePixel(int x, int y, Color c);
    /**
     * @brief Return a pointer to the color at coordinates x and y
     * @param x x coordinate
     * @param y y coordinate
     * @return color
     */
    Color pixelAt(int x, int y);
    /**
     * @brief Return the Portable Pixmap representation of this Canvas
     * @return Portable Pixmap representation of this Canvas
     */
    std::string toPortablePixmap();

  private:
    std::vector<std::vector<Color>> pixels;
  };
}  // namespace raytracerchallenge
