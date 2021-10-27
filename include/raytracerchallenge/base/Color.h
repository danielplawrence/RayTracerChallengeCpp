#pragma once

namespace raytracerchallenge {
  /**
   * @brief Represents an RGB color.
   */
  class Color {
  public:
    double red{};
    double green{};
    double blue{};
    /**
     * Default constructor
     */
    Color();
    /**
     * Constructor for a Color
     * @param red red value
     * @param green green value
     * @param blue value
     */
    Color(double red, double green, double blue);
    /**
     * @brief binary subtraction operator
     * @param c target for subtraction
     * @return difference of the operands
     */
    Color operator-(const Color &c) const;
    /**
     * @brief binary addition operator
     * @param c target for addition
     * @return sum of the operands
     */
    Color operator+(const Color &c) const;
    /**
     * @brief binary multiplication operator
     * @param c target for multiplication
     * @return product of the operands
     */
    Color operator*(const Color &c) const;
    /**
     * @brief scalar multiplication operator
     * @param f target for multiplication
     * @return a new Color multiplied by f
     */
    Color operator*(double f) const;
    /**
     * @brief equality operator
     * @param c target for comparison
     * @return true if the colors have the same red, green and blue values
     */
    bool operator==(const Color &c) const;
  };
  static const Color BLACK = {0.0, 0.0, 0.0};
  static const Color WHITE = {1.0, 1.0, 1.0};
}  // namespace raytracerchallenge