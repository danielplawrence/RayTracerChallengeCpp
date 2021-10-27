#pragma once

#include <Eigen/Dense>
#include <vector>

#include "Tuple.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

namespace raytracerchallenge {
  /**
   * @brief Represents a matrix of doubles
   */
  class Matrix {
  public:
    MatrixXd m;
    /**
     * @brief Default constructor
     */
    Matrix();
    /**
     * Copy constructor from MatrixXd
     * @param m base MatrixXd
     */
    explicit Matrix(MatrixXd m);
    /**
     * @brief Construct a Matrix given a 2D vector and its dimensions
     * @param x height of the input vector
     * @param width of the input vector
     * @param m a 2D vector providing initial values for the Matrix
     */
    Matrix(unsigned int x, unsigned int y, std::vector<std::vector<double>> m);
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
    [[nodiscard]] double determinant() const;
    /**
     * @brief Return true if the Matrix is invertible
     * @return true if the Matrix is invertible
     */
    [[nodiscard]] bool invertible() const;
    /**
     * @brief Return the inverse of this Matrix
     * @return the inverse of this Matrix
     */
    [[nodiscard]] Matrix inverse() const;
    /**
     * @brief Translate this matrix using the provided x, y, z values
     * @param x value for x
     * @param y value for y
     * @param z value for z
     * @return translated matrix
     */
    [[nodiscard]] Matrix translated(double x, double y, double z) const;
    /**
     * @brief Scale this matrix using the provided x, y, z values
     * @param x value for x
     * @param y value for y
     * @param z value for z
     * @return scaled matrix
     */
    [[nodiscard]] Matrix scaled(double x, double y, double z) const;
    /**
     * @brief Rotate this matrix for the provided radians on the X axis
     * @param radians
     * @return X-rotated matrix
     */
    [[nodiscard]] Matrix rotatedX(double radians) const;
    /**
     * @brief Rotate this matrix for the provided radians on the Y axis
     * @param radians
     * @return Y-rotated matrix
     */
    [[nodiscard]] Matrix rotatedY(double radians) const;
    /**
     * @brief Rotate this matrix for the provided radians on the Z axis
     * @param radians
     * @return Z-rotated matrix
     */
    [[nodiscard]] Matrix rotatedZ(double radians) const;
    /**
     * @brief Shear this matrix using the provided params
     * @param xy scaling of x relative to y
     * @param xz scaling of x relative to z
     * @param yx scaling of y relative to x
     * @param yz scaling of y relative to z
     * @param zx scaling of z relative to x
     * @param zy scaling of z relative to y
     * @return sheared matrix
     */
    [[nodiscard]] Matrix sheared(double xy, double xz, double yx, double yz, double zx,
                                 double zy) const;
    /**
     * @brief Return a translation matrix for the provided x, y, z values
     * @param x value for x
     * @param y value for y
     * @param z value for z
     * @return translation matrix
     */
    static Matrix translation(double x, double y, double z);
    /**
     * @brief Return a scaling matrix for the provided x, y, z values
     * @param x value for x
     * @param y value for y
     * @param z value for z
     * @return scaling matrix
     */
    static Matrix scaling(double x, double y, double z);
    /**
     * @brief Return an X-rotation matrix for the provided radians
     * @param radians
     * @return X-rotation matrix
     */
    static Matrix rotationX(double radians);
    /**
     * @brief Return a Y-rotation matrix for the provided radians
     * @param radians
     * @return Y-rotation matrix
     */
    static Matrix rotationY(double radians);
    /**
     * @brief Return a Z-rotation matrix for the provided radians
     * @param radians
     * @return Z-rotation matrix
     */
    static Matrix rotationZ(double radians);
    /**
     * @brief Return a shearing matrix for the provided params
     * @param xy scaling of x relative to y
     * @param xz scaling of x relative to z
     * @param yx scaling of y relative to x
     * @param yz scaling of y relative to z
     * @param zx scaling of z relative to x
     * @param zy scaling of z relative to y
     * @return shearing matrix
     */
    static Matrix shearing(double xy, double xz, double yx, double yz, double zx, double zy);
    /**
     * @brief Return the view transform
     * @param from location of eye
     * @param to point where eye is looking
     * @param up vector representing up
     * @return View transform
     */
    static Matrix view(Tuple from, Tuple to, Tuple up);
  };
}  // namespace raytracerchallenge