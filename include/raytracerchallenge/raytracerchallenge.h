#pragma once

#include <string>

namespace raytracerchallenge {
  /**
   * @brief A 3D renderer
   */
  class RayTracerChallenge {
  public:
    /**
     * @brief A class representing a tuple (x, y, z, w)
     */
    class Tuple {
    public:
      float x{};
      float y{};
      float z{};
      float w{};
      [[nodiscard]] bool isVector() const;
      [[nodiscard]] float magnitude() const;
      /**
       * @brief create a new Tuple
       * @param x position on the x axis
       * @param y position on the y axis
       * @param z position on the z axis
       * @param w 0 for a vector; 1 for a point
       */
      Tuple(float x, float y, float z, float w);
      /**
       * @brief create a point
       * @param x position on the x axis
       * @param y position on the y axis
       * @param z position on the z axis
       */
      static Tuple point(float x, float y, float z);
      /**
       * @brief create a vector
       * @param x position on the x axis
       * @param y position on the y axis
       * @param z position on the z axis
       */
      static Tuple vector(float x, float y, float z);
      /**
       * @brief equality operator
       * @param t target for comparison
       * @return true if the tuples have the same x, y and x values
       */
      bool operator==(const Tuple &t) const;
      /**
       * @brief addition operator
       * @param t target for addition
       * @return sum of the operands
       */
      Tuple operator+(const Tuple &t) const;
      /**
       * @brief binary subtraction operator
       * @param t target for subtraction
       * @return difference of the operands
       */
      Tuple operator-(const Tuple &t) const;
      /**
       * @brief unary subtraction operator
       * @return inverse of the operand
       */
      Tuple operator-() const;
      /**
       * @brief scalar multiplication operator
       * @return a new Tuple multiplied by f
       */
      Tuple operator*(float f) const;
      /**
       * @brief scalar division operator
       * @param f divisor
       * @return a new Tuple divided by f
       */
      Tuple operator/(float f) const;
      /**
       * @brief normalize the Tuple
       * @return normalized Tuple
       */
      [[nodiscard]] Tuple normalize() const;
      /**
       * @brief the dot product of this Tuple and another
       * @param t Tuple to compute dot product with
       * @return scalar value representing the dot product of the two tuples
       */
      [[nodiscard]] float dot(const Tuple &t) const;
      /**
       * @brief the cross product of this Tuple and another
       * @param t Tuple to compute cross product with
       * @return new Tuple representing the cross product of this tuple and another
       */
      [[nodiscard]] Tuple cross(const Tuple &t) const;

    private:
      static bool floatEquals(float x, float y);
    };
    /**
     * Represents an RGB Color
     */
    class Color {
    public:
      float red{};
      float green{};
      float blue{};
      /**
       * Constructor for a Color
       * @param red red value
       * @param green green value
       * @param blue value
       */
      Color(float red, float green, float blue);
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
      Color operator*(float f) const;
      /**
       * @brief equality operator
       * @param c target for comparison
       * @return true if the colors have the same red, green and blue values
       */
      bool operator==(const Color &c) const;
    };
  };
}  // namespace raytracerchallenge
