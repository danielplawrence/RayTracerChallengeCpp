#pragma once

#include <string>

namespace raytracerchallenge {
  /**
   * @brief A class for saying hello in multiple languages
   */
  class RayTracerChallenge {
    std::string name;

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
       * @brief create a new tuple
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
       * @return sum of the operannds
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
       * @param f
       * @return a new Tuple divided by f
       */
      Tuple operator/(float f) const;
      /**
       * @brief normalize the tuple
       * @return normalized tuple
       */
      [[nodiscard]] Tuple normalize() const;
      /**
       * @brief the dot product of this tuple and another
       * @param t tuple to compute dot product with
       * @return scalar value representing the dot product of the two tuples
       */
      [[nodiscard]] float dot(const Tuple &t) const;
      /**
       * @brief the cross product of this tuple and another
       * @param t tuple to compute cross product with
       * @return new tuple representing the cross product of this tuple and another
       */
      [[nodiscard]] Tuple cross(const Tuple &t) const;

    private:
      static bool floatEquals(float x, float y);
    };
  };
}  // namespace raytracerchallenge
