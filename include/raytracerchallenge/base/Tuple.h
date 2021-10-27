#pragma once

namespace raytracerchallenge {
  /**
   * @brief A class representing a tuple (x, y, z, w)
   */
  class Tuple {
  public:
    double x{};
    double y{};
    double z{};
    double w{};
    [[nodiscard]] bool isVector() const;
    [[nodiscard]] double magnitude() const;
    /**
     * @brief default constructor for Tuples
     */
    Tuple();
    /**
     * @brief create a new Tuple
     * @param x position on the x axis
     * @param y position on the y axis
     * @param z position on the z axis
     * @param w 0 for a vector; 1 for a point
     */
    Tuple(double x, double y, double z, double w);
    /**
     * @brief create a point
     * @param x position on the x axis
     * @param y position on the y axis
     * @param z position on the z axis
     */
    static Tuple point(double x, double y, double z);
    /**
     * @brief create a vector
     * @param x position on the x axis
     * @param y position on the y axis
     * @param z position on the z axis
     */
    static Tuple vector(double x, double y, double z);
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
    Tuple operator*(double f) const;
    /**
     * @brief scalar division operator
     * @param f divisor
     * @return a new Tuple divided by f
     */
    Tuple operator/(double) const;
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
    [[nodiscard]] double dot(const Tuple &t) const;
    /**
     * @brief the cross product of this Tuple and another
     * @param t Tuple to compute cross product with
     * @return new Tuple representing the cross product of this tuple and another
     */
    [[nodiscard]] Tuple cross(const Tuple &t) const;
    /**
     * @brief return the reflection of this tuple around another
     * @param t Tuple to reflect this one around
     * @return reflected Tuple
     */
    [[nodiscard]] Tuple reflect(const Tuple &t) const;
  };
}  // namespace raytracerchallenge