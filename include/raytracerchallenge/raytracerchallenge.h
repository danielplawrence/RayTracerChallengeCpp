#pragma once

#include <string>
#include <vector>

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
       * Default constructor
       */
      Color();
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
    /**
     * @brief Represents a matrix of floating-point numbers
     */
    class Matrix {
      std::vector<std::vector<float>> m;

    public:
      /**
       * @brief The row of a Matrix
       */
      class Row {
        std::vector<float> row;

      public:
        /**
         * @brief Row index
         */
        float operator[](unsigned int y);
        /**
         * @brief Construct a row from a vector of floating-point numbers
         * @param r Vector to create this row from
         */
        explicit Row(std::vector<float> r);
      };
      /**
       * @brief Construct a Matrix given a 2D vector and its dimensions
       * @param x height of the input vector
       * @param width of the input vector
       * @param m a 2D vector providing initial values for the Matrix
       */
      Matrix(unsigned int x, unsigned int y, std::vector<std::vector<float>> m);
      /**
       * @brief return a Row of the Matrix at index x
       * @param x row index
       * @return Row
       */
      Row operator[](unsigned int x) const;
      /**
       * @brief Matrix equality operator
       * @param matrix Matrix for comparison
       * @return True if the elements of both Matrices are equal
       */
      bool operator==(const Matrix &matrix) const;
      /**
       * @brief Matrix inequality operator
       * @param matrix Matrix for comparison
       * @return True if the elements of both Matrices are not equal
       */
      bool operator!=(const Matrix &matrix) const;
      /**
       * @brief Matrix multiplication
       * @param matrix Matrix to be multiplied with this one
       * @return product of the two Matrices
       */
      Matrix operator*(const Matrix &matrix) const;
      /**
       * @brief Matrix/Tuple multiplication
       * @param tuple Tuple to be multiplied with this Matrix
       * @return Tuple representing the multiplication result
       */
      Tuple operator*(const Tuple &tuple) const;
      /**
       * @brief Identity Matrix
       * @param size size for the Matrix
       * @return a square identity Matrix
       */
      static Matrix identity(unsigned int size);
      /**
       * @brief Return the transpose of this Matrix
       * @return Transpose of this Matrix
       */
      Matrix transposed();
      /**
       * @brief Return the determinant of this Matrix
       * @return the determinant
       */
      float determinant();
      /**
       * @brief Return the submatrix which results fromm removing
       * the column at index xy
       * @param x row to remove
       * @param y column to remove
       * @return this Matrix with row x and column y removed
       */
      Matrix submatrix(unsigned int x, unsigned int y);
      /**
       * @brief Return the minor of the element at index x,y
       * @param x x index
       * @param y index
       * @return the minor of the element at index x,y
       */
      float minor(unsigned int x, unsigned int y);
      /**
       * @brief Return the cofactor of the element at index x,y
       * @param x x index
       * @param y index
       * @return the cofactor of the element at index x,y
       */
      float cofactor(unsigned int x, unsigned int y);
      /**
       * @brief Return true if the Matrix is invertible
       * @return true if the Matrix is invertible
       */
      bool invertible();
      /**
       * @brief Return the inverse of this Matrix
       * @return the inverse of this Matrix
       */
      Matrix inverse();
      /**
       * Return a translation matrix for the provided x, y, z values
       * @param x value for x
       * @param y value for y
       * @param z value for z
       * @return translation matrix
       */
      static Matrix translation(float x, float y, float z);
      /**
       * Return a scaling matrix for the provided x, y, z values
       * @param x value for x
       * @param y value for y
       * @param z value for z
       * @return scaling matrix
       */
      static Matrix scaling(float x, float y, float z);

    private:
      static bool floatEquals(float x, float y);
    };
  };
}  // namespace raytracerchallenge
