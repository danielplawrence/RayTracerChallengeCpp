#pragma once

#include <cmath>
#include <optional>
#include <set>
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
      /**
       * @brief return the reflection of this tuple around another
       * @param t Tuple to reflect this one around
       * @return reflected Tuple
       */
      [[nodiscard]] Tuple reflect(const Tuple &t) const;

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
       * Default constructor
       */
      Matrix();
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
       * @brief Translate this matrix using the provided x, y, z values
       * @param x value for x
       * @param y value for y
       * @param z value for z
       * @return translated matrix
       */
      [[nodiscard]] Matrix translated(float x, float y, float z) const;
      /**
       * @brief Scale this matrix using the provided x, y, z values
       * @param x value for x
       * @param y value for y
       * @param z value for z
       * @return scaled matrix
       */
      [[nodiscard]] Matrix scaled(float x, float y, float z) const;
      /**
       * @brief Rotate this matrix for the provided radians on the X axis
       * @param radians
       * @return X-rotated matrix
       */
      [[nodiscard]] Matrix rotatedX(float radians) const;
      /**
       * @brief Rotate this matrix for the provided radians on the Y axis
       * @param radians
       * @return Y-rotated matrix
       */
      [[nodiscard]] Matrix rotatedY(float radians) const;
      /**
       * @brief Rotate this matrix for the provided radians on the Z axis
       * @param radians
       * @return Z-rotated matrix
       */
      [[nodiscard]] Matrix rotatedZ(float radians) const;
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
      [[nodiscard]] Matrix sheared(float xy, float xz, float yx, float yz, float zx,
                                   float zy) const;
      /**
       * @brief Return a translation matrix for the provided x, y, z values
       * @param x value for x
       * @param y value for y
       * @param z value for z
       * @return translation matrix
       */
      static Matrix translation(float x, float y, float z);
      /**
       * @brief Return a scaling matrix for the provided x, y, z values
       * @param x value for x
       * @param y value for y
       * @param z value for z
       * @return scaling matrix
       */
      static Matrix scaling(float x, float y, float z);
      /**
       * @brief Return an X-rotation matrix for the provided radians
       * @param radians
       * @return X-rotation matrix
       */
      static Matrix rotationX(float radians);
      /**
       * @brief Return a Y-rotation matrix for the provided radians
       * @param radians
       * @return Y-rotation matrix
       */
      static Matrix rotationY(float radians);
      /**
       * @brief Return a Z-rotation matrix for the provided radians
       * @param radians
       * @return Z-rotation matrix
       */
      static Matrix rotationZ(float radians);
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
      static Matrix shearing(float xy, float xz, float yx, float yz, float zx, float zy);
      /**
       * @brief Return the view transform
       * @param from location of eye
       * @param to point where eye is looking
       * @param up vector representing up
       * @return View transform
       */
      static Matrix view(Tuple from, Tuple to, Tuple up);

    private:
      static bool floatEquals(float x, float y);
    };
    /**
     * @brief Represents a ray of light
     */
    class Ray {
    public:
      /**
       * @brief Starting point of ray
       */
      Tuple origin;
      /**
       * @brief Vector representing the ray's direction
       */
      Tuple direction;
      /**
       * @brief Construct a ray of light
       * @param origin origin of the ray
       * @param direction direction of the ray
       */
      Ray(Tuple origin, Tuple direction);
      /**
       * @brief Return the position at point t along the ray
       */
      [[nodiscard]] Tuple position(float t) const;
      /**
       * @brief Return the transformation of this ray using the provided transformation matrix
       * @param matrix transformation matrix
       * @return a new Ray with the transformation applied
       */
      [[nodiscard]] Ray transform(const Matrix &matrix) const;
    };
    /**
     * @brief Represents a material
     */
    class Material {
    public:
      float ambient = 0.1f;
      float diffuse = 0.9f;
      float specular = 0.9f;
      float shininess = 200.0f;
      Color color = Color(1.0f, 1.0f, 1.0f);
      bool operator==(const Material &material) const;
    };
    class Intersection;
    class Intersections;
    /**
     * @brief Base class for objects
     */
    class Object {
    public:
      /**
       * @brief A unique id for this object
       */
      std::string id;
      /**
       * @brief Transformation matrix of this object
       */
      Matrix transform;
      /**
       * @brief Material for this object
       */
      Material material;
      /**
       * @brief Return all intersections between this object
       * and the provided ray
       * @param ray
       * @return collection of Intersections representing positions where
       * the ray passed throough this object
       */
      Intersections intersect(Ray ray);
      /**
       * @brief Return the normal vector at the specified point on an object
       * @param point
       * @return normal vector
       */
      Tuple normalAt(Tuple point);
      /**
       * @brief Default constructor
       */
      Object();
      /**
       * @brief Equality operator. Objects are equal if they share the same properties
       * @param object object to compare with this one
       * @return true if the objects share the same properties
       */
      bool operator==(const Object &object) const;
      /**
       * @brief Less than operator; just uses the ID
       * @param object object to compare with this one
       * @return true if this object is less than the target
       */
      bool operator<(const Object &object) const;
      /**
       * @brief Return true if the objects share the same ID
       * @param object Object for comparison
       * @return true if the objects share the same ID
       */
      [[nodiscard]] bool is(const Object &object) const;
    };
    /**
     * @brief Represents a sphere
     */
    class [[maybe_unused]] Sphere : public Object{public : };
    /**
     * Ray intersection computations
     */
    class Computations {
    public:
      float t{};
      Object object;
      Tuple point;
      Tuple overPoint;
      Tuple eyeVector;
      Tuple normalVector;
      bool inside{};
    };
    /**
     * @brief Represents an intersection between a ray an object
     */
    class Intersection {
    public:
      /**
       * @brief the point on the ray where it intersected with an object
       */
      float t{};
      /**
       * @brief the object that intersected with the ray
       */
      Object object;
      /**
       * @brief Construct a new Intersection
       * @param t the point where this intersection occurred on a ray
       * @param object the object which intersected with the ray
       */
      Intersection(float t, Object object);
      /**
       * @brief Default constructtor
       */
      Intersection();
      /**
       * @brief Prepare ray intersection computations
       * @param ray
       * @return Computations
       */
      Computations prepareComputations(Ray ray);
      /**
       * @brief Equality operator
       * @param intersection intersection to compare with this one
       * @return true if the intersections have the same properties
       */
      bool operator==(const Intersection &intersection) const;
      /**
       * @brief Less than operator. Comparison is based on the 't' property
       * @param intersection intersection to compare with this one
       * @return true if the t property of this intersection is smaller than that of the argument
       */
      bool operator<(const Intersection &intersection) const;
    };
    /**
     * @brief A collection of Intersection objects
     */
    class Intersections {
    public:
      /**
       * @brief Return the smallest non-negative intersection in the collection
       * @return the smallest non-negative intersection in the collection
       */
      std::optional<Intersection> hit();
      /**
       * @brief Construct a new Intersections
       * @param intersections to be wrapped by this container
       */
      explicit Intersections(std::vector<Intersection> intersections);
      /**
       * @brief Default constructor
       */
      Intersections();
      /**
       * @brief Index operator
       * @param x index
       * @return intersection at index x
       */
      Intersection operator[](unsigned int x) const;
      /**
       * @brief Add the content of an Intersections to this one.
       * @param newIntersections Intersections to be added.
       */
      void addAll(Intersections newIntersections);
      /**
       * @brief Sort the intersections
       */
      void sort();
      /**
       * @brief Return the number of intersections in the collection
       * @return number of intersections in the collection
       */
      [[nodiscard]] size_t size() const;

    private:
      std::vector<Intersection> intersections;
    };
    /**
     * @brief Represents a light source
     */
    class PointLight {
    public:
      Color intensity;
      Tuple position;
      /**
       * @brief Construct a new PointLight
       * @param position Tuple representing the position of the light
       * @param intensity Color of the light
       */
      PointLight(Tuple position, Color intensity);
    };
    class World {
    public:
      std::vector<Object> objects;
      std::optional<PointLight> light;
      /**
       * Default constructor for a World
       */
      World();
      /**
       * Return true if this object is in the World
       * @param object Object to test
       * @return true if this object is in the World
       */
      bool contains(Object &object);
      /**
       * Return true if the world contains no objects
       * @return true if the world contains no objects
       */
      [[nodiscard]] bool isEmpty() const;
      /**
       * Add an Object to the world
       * @param object target Object
       */
      void add(Object &object);
      /**
       * Intersect this world with a ray
       * @param ray to pass through the world
       * @return intersections between the ray
       * and objects in this world
       */
      Intersections intersect(Ray ray);
      /**
       * @brief Return the default World
       * @return Default World
       */
      static World defaultWorld();
      /**
       * @brief Return the color at the intersection encapsulated
       * by computations in this world
       * @param computations
       * @return color
       */
      Color shadeHit(const Computations &computations);
      /**
       * Intersect the world with the given ray and return
       * the color at the resulting intersection
       * @param ray Ray to intersect with this World
       * @return Color at the resulting intersection
       */
      Color colorAt(Ray ray);
      /**
       * Return True if this point is in shadow
       * @param point to check for shadow
       * @return True if this point is in shadow
       */
      bool isShadowed(Tuple point);
    };
    /**
     * Represents a Camera
     */
    class Camera {
    public:
      int hSize;
      int vSize;
      float pixelSize;
      float fieldOfView;
      float halfWidth;
      float halfHeight;
      Matrix transform;
      /**
       * Create a new camera
       * @param hSize horizontal size of canvas
       * @param vSize vertical size of canvas
       * @param fieldOfView camera angle
       */
      Camera(int hSize, int vSize, float fieldOfView);
      /**
       * Return a ray targeting the pixel at this position
       * @param x X position
       * @param y Y positionim
       * @return Ray
       */
      Ray rayForPixel(int x, int y);
      /**
       * Render a world using this camera
       * @param world The World to render
       * @return a Canvas containing the rendered image
       */
      Canvas render(World world);
    };
    /**
     * Calculate the lighting at a particular position on a material
     * using the Phong reflection model
     * @param material Target material
     * @param light Light source
     * @param position Position we need the lighting for
     * @param eyeVector Eye vector
     * @param normalVector Normal vector
     * @param inShadow Whether or not the position is in shadow
     * @return The color for the target position
     */
    static Color lighting(Material material, PointLight light, Tuple position, Tuple eyeVector,
                          Tuple normalVector, bool inShadow);
  };
}  // namespace raytracerchallenge
