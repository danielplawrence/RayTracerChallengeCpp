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

    private:
      static bool floatEquals(float x, float y);
    };
  };
}  // namespace raytracerchallenge
