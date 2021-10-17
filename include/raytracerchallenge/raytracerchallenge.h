#pragma once

#include <Eigen/Dense>
#include <cmath>
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using Eigen::MatrixXd;

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

    private:
      static bool doubleEquals(double x, double y);
    };
    /**
     * Represents an RGB Color
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
      static const Color BLACK;
      static const Color WHITE;
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
     * @brief Represents a matrix of doubles
     */
    class Matrix {
    public:
      MatrixXd m;
      /**
       * Default constructor
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
      [[nodiscard]] RayTracerChallenge::Matrix inverse() const;
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
      [[nodiscard]] Tuple position(double t) const;
      /**
       * @brief Return the transformation of this ray using the provided transformation matrix
       * @param matrix transformation matrix
       * @return a new Ray with the transformation applied
       */
      [[nodiscard]] Ray transform(const Matrix &matrix) const;
    };
    class Pattern;
    /**
     * @brief Represents a material
     */
    class Material {
    public:
      Pattern *pattern = nullptr;
      double ambient = 0.1;
      double diffuse = 0.9;
      double specular = 0.9;
      double shininess = 200.0;
      double reflective = 0.0;
      double transparency = 0.0;
      double refractiveIndex = 1.0;
      bool castShadow = true;
      Color color = Color(1.0, 1.0, 1.0);
      bool operator==(const Material &material) const;
    };
    class Intersection;
    class Intersections;
    class Group;
    class BoundingBox;
    /**
     * @brief Base class for objects
     */
    class Shape {
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
       * @brief The parent of this object
       */
      std::shared_ptr<Shape> parent = nullptr;
      /**
       * @brief Return all intersections between this object
       * and the provided ray
       * @param ray
       * @return collection of Intersections representing positions where
       * the ray passed through this object
       */
      Intersections intersect(Ray ray);
      /**
       * @brief Implementation-specific logic for returning all intersections
       * between a shape and a ray. Defaults to Sphere behavior.
       * @param ray
       * @return collection of Intersections representing positions where
       * the ray passed through this object
       */
      virtual Intersections localIntersect(Ray ray) = 0;
      /**
       * @brief Return the normal vector at the specified point on an object
       * @param point
       * @return normal vector
       */
      Tuple normalAt(Tuple point, Intersection hit);
      /**
       * @brief Implementation-specific logic for returning the normal at a specific
       * point on this object.
       * @param point
       * @return the normal vector at the specified point on this object
       */
      virtual Tuple localNormalAt(Tuple point, Intersection hit) = 0;
      /**
       * @brief Return the bounds for this shape
       * @return bounds for this shape
       */
      virtual BoundingBox bounds() = 0;
      /**
       * @brief Return the bounding box of this object in parent space
       * @return The bounding box of this object
       */
      BoundingBox parentSpaceBounds();
      /**
       * Helper for converting from world space to object space
       * @param point Point in world space
       * @return point in object space
       */
      [[nodiscard]] Tuple worldToObject(Tuple point) const;
      /**
       * Helper for converting a normal vector in object space to
       * a normal vector in world space
       * @param normal Normal vector in object space
       * @return Normal vector in world space
       */
      [[nodiscard]] Tuple normalToWorld(Tuple normal) const;
      /**
       * @brief Default constructor
       */
      Shape();
      /**
       * @brief Equality operator. Objects are equal if they share the same properties
       * @param object object to compare with this one
       * @return true if the objects share the same properties
       */
      bool operator==(const Shape &object) const;
      /**
       * @brief Less than operator; just uses the ID
       * @param object object to compare with this one
       * @return true if this object is less than the target
       */
      bool operator<(const Shape &object) const;
      /**
       * @brief Return true if the objects share the same ID
       * @param object Object for comparison
       * @return true if the objects share the same ID
       */
      [[nodiscard]] bool is(const Shape &object) const;
      /**
       * Return true if this shape includes another shape.
       * The default definition of 'includes' is equality,
       * but subtypes may differ in their implementation.
       * @param object Object for comparison
       * @return true if this shape includes the target.
       */
      virtual bool includes(const Shape &object) const;
      virtual ~Shape() = default;

      std::shared_ptr<Shape> sharedPtr;
    };
    /**
     * @brief Describes the bounding box of an object
     */
    class BoundingBox {
    public:
      /* Point representing the minimum bound */
      Tuple min = Tuple::point(INFINITY, INFINITY, INFINITY);
      /* Point representing the maximum bound */
      Tuple max = Tuple::point(double(-INFINITY), double(-INFINITY), double(-INFINITY));
      /**
       * @brief Default constructor
       */
      BoundingBox() = default;
      /**
       * @brief Constructor from min and max points
       * @param min Min point of the box
       * @param max Max point of the box
       */
      BoundingBox(Tuple min, Tuple max) {
        this->min = min;
        this->max = max;
      }
      /**
       * @brief Add a new point to the box, extending
       * min and max bounds as necessary
       */
      void add(Tuple point);
      /**
       * @brief Add a new box to the box, extending
       * min and max bounds as necessary
       * @param box
       */
      void add(BoundingBox box);
      /**
       * @brief Returns true if the provided point
       * is inside the bounding box
       * @param point
       * @return true if point is inside this box
       */
      [[nodiscard]] bool contains(Tuple point) const;
      /**
       * @brief Returns true if the provided box
       * is contained within this box
       * @param box
       * @return true if box is inside this box
       */
      [[nodiscard]] bool contains(BoundingBox box) const;
      /**
       * @brief Apply a transform to this box
       * @param matrix Transformation matrix
       * @return Transformed box
       */
      BoundingBox transform(Matrix matrix) const;
      /**
       * @brief Check if a ray intersects this boc
       * @param ray
       * @return True if ray intersects this box
       */
      bool intersects(Ray ray);
    };
    /**
     * @brief Represents a group of objects
     */
    class Group : public Shape {
    public:
      /* Objects contained in this group */
      std::vector<std::shared_ptr<Shape>> objects;
      /**
       * @brief Factory method
       * @return a pointer to a new Group
       */
      static std::shared_ptr<Shape> create() {
        auto shape = new Group();
        return shape->sharedPtr;
      }
      /**
       * @brief Add an object to the group
       * @param object pointer to target object
       */
      void add(const std::shared_ptr<Shape> &object);
      BoundingBox bounds() override;
      Tuple localNormalAt(Tuple point, Intersection hit) override;
      Intersections localIntersect(Ray ray) override;
      bool includes(const RayTracerChallenge::Shape &object) const override;
    };
    /**
     * @brief Represents a sphere
     */
    class Sphere : public Shape {
    public:
      Sphere() = default;
      static std::shared_ptr<Shape> create() {
        auto shape = new Sphere();
        return shape->sharedPtr;
      }
      Tuple localNormalAt(Tuple point, Intersection hit) override;
      Intersections localIntersect(Ray ray) override;
      BoundingBox bounds() override;
    };
    /**
     * @brief A glass sphere
     */
    class GlassSphere : public Sphere {
    public:
      GlassSphere() {
        this->material.transparency = 1.0;
        this->material.refractiveIndex = 1.5;
      }
      static std::shared_ptr<Shape> create() {
        auto shape = new GlassSphere();
        return shape->sharedPtr;
      }
    };
    /**
     * @brief Represents a flat surface
     */
    class Plane : public Shape {
    public:
      Plane() = default;
      static std::shared_ptr<Shape> create() {
        auto shape = new Plane();
        return shape->sharedPtr;
      }
      Tuple localNormalAt(Tuple point, Intersection hit) override;
      Intersections localIntersect(Ray ray) override;
      BoundingBox bounds() override;
    };
    /**
     * @brief Represents a cube
     */
    class Cube : public Shape {
    public:
      Cube() = default;
      static std::shared_ptr<Shape> create() {
        auto shape = new Cube();
        return shape->sharedPtr;
      }
      Tuple localNormalAt(Tuple point, Intersection hit) override;
      Intersections localIntersect(Ray ray) override;
      BoundingBox bounds() override;
    };
    /**
     * @brief Represents a cylinder
     */
    class Cylinder : public Shape {
    public:
      Cylinder() = default;
      static std::shared_ptr<Shape> create(double min = double(-INFINITY), double max = INFINITY,
                                           bool closed = false) {
        auto shape = new Cylinder();
        shape->maximum = max;
        shape->minimum = min;
        shape->closed = closed;
        return shape->sharedPtr;
      }
      Tuple localNormalAt(Tuple point, Intersection hit) override;
      Intersections localIntersect(Ray ray) override;
      BoundingBox bounds() override;
      double minimum = double(-INFINITY);
      double maximum = double(INFINITY);
      bool closed = false;
    };
    /**
     * @brief Represents a cone
     */
    class Cone : public Cylinder {
    public:
      static std::shared_ptr<Shape> create(double min = double(-INFINITY), double max = INFINITY,
                                           bool closed = false) {
        auto shape = new Cone();
        shape->maximum = max;
        shape->minimum = min;
        shape->closed = closed;
        return shape->sharedPtr;
      }
      Tuple localNormalAt(Tuple point, Intersection hit) override;
      Intersections localIntersect(Ray ray) override;
      BoundingBox bounds() override;
    };
    /**
     * @brief Represents a triangle
     */
    class Triangle : public Shape {
    public:
      Tuple p1;
      Tuple p2;
      Tuple p3;
      Tuple e1;
      Tuple e2;
      Tuple normal;
      Triangle() = default;
      Triangle(Tuple p1, Tuple p2, Tuple p3) {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        this->e1 = p2 - p1;
        this->e2 = p3 - p1;
        this->normal = e2.cross(e1).normalize();
      }
      static std::shared_ptr<Shape> create(Tuple p1, Tuple p2, Tuple p3) {
        return (new Triangle(p1, p2, p3))->sharedPtr;
      }
      Tuple localNormalAt(Tuple point, Intersection hit) override;
      Intersections localIntersect(Ray ray) override;
      BoundingBox bounds() override;
    };
    class SmoothTriangle : public Triangle {
    public:
      Tuple n1;
      Tuple n2;
      Tuple n3;
      SmoothTriangle(Tuple p1, Tuple p2, Tuple p3, Tuple n1, Tuple n2, Tuple n3) {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        this->n1 = n1;
        this->n2 = n2;
        this->n3 = n3;
        this->e1 = p2 - p1;
        this->e2 = p3 - p1;
      }
      static std::shared_ptr<Shape> create(Tuple p1, Tuple p2, Tuple p3, Tuple n1, Tuple n2,
                                           Tuple n3) {
        return (new SmoothTriangle(p1, p2, p3, n1, n2, n3))->sharedPtr;
      }
      Tuple localNormalAt(Tuple point, Intersection hit) override;
      BoundingBox bounds() override;
    };
    class CSG : public Shape {
    public:
      enum Operation { Union, Intersection, Difference };
      Operation operation;
      std::shared_ptr<Shape> left;
      std::shared_ptr<Shape> right;
      CSG(std::shared_ptr<Shape> &shape1, std::shared_ptr<Shape> &shape2, Operation operation) {
        this->left = shape1;
        this->right = shape2;
        shape1->parent = this->sharedPtr;
        shape2->parent = this->sharedPtr;
        this->operation = operation;
      }
      static std::shared_ptr<Shape> create(std::shared_ptr<Shape> &shape1,
                                           std::shared_ptr<Shape> &shape2,
                                           Operation operation = Union) {
        return (new CSG(shape1, shape2, operation))->sharedPtr;
      }
      Tuple localNormalAt(Tuple point, class Intersection hit) override;
      Intersections localIntersect(Ray ray) override;
      BoundingBox bounds() override;
      Intersections filterIntersections(Intersections intersections) const;
      bool includes(const Shape &object) const override;
      static bool intersectionAllowed(Operation op, bool leftHit, bool inLeft, bool inRight);
    };
    /**
     * A Pattern accepts a point in space and returns a color
     */
    class Pattern {
    public:
      Matrix transform = Matrix::identity(4);
      /**
       * Get the color at this point on this shape
       * @param shape
       * @param point
       * @return Color
       */
      [[nodiscard]] virtual Color colorAt(std::shared_ptr<Shape> shape, Tuple point) const = 0;
    };
    /**
     * Represents alternating stripes
     */
    class StripePattern : public Pattern {
    private:
      Color a;
      Color b;

    public:
      StripePattern(Color a, Color b);
      [[nodiscard]] Color colorAt(std::shared_ptr<Shape> shape, Tuple point) const override;
    };
    /**
     * Represents a gradient
     */
    class GradientPattern : public Pattern {
    private:
      Color a;
      Color b;

    public:
      GradientPattern(Color a, Color b);
      [[nodiscard]] Color colorAt(std::shared_ptr<Shape> shape, Tuple point) const override;
    };
    /**
     * Represents a ring pattern
     */
    class RingPattern : public Pattern {
    private:
      Color a;
      Color b;

    public:
      RingPattern(Color a, Color b);
      [[nodiscard]] Color colorAt(std::shared_ptr<Shape> shape, Tuple point) const override;
    };
    /**
     * Represents a checkers pattern
     */
    class CheckersPattern : public Pattern {
    private:
      Color a;
      Color b;

    public:
      CheckersPattern(Color a, Color b);
      [[nodiscard]] Color colorAt(std::shared_ptr<Shape> shape, Tuple point) const override;
    };
    /**
     * Ray intersection computations
     */
    class Computations {
    public:
      double t{};
      std::shared_ptr<Shape> object;
      Tuple point;
      Tuple overPoint;
      Tuple underPoint;
      Tuple eyeVector;
      Tuple normalVector;
      Tuple reflectionVector;
      double n1;
      double n2;
      bool inside{};
      /**
       * Calculate the Schlick approximation for these computations
       * @param computations
       * @return schlick approximation
       */
      static double schlick(const Computations &computations);
    };
    /**
     * @brief Represents an intersection between a ray an object
     */
    class Intersection {
    public:
      /**
       * @brief the point on the ray where it intersected with an object
       */
      double t{};
      double u{};
      double v{};
      /**
       * @brief the object that intersected with the ray
       */
      std::shared_ptr<Shape> object;
      /**
       * @brief Construct a new Intersection
       * @param t the point where this intersection occurred on a ray
       * @param object the object which intersected with the ray
       */
      Intersection(double t, std::shared_ptr<Shape> object);
      /**
       * @brief Default constructor
       */
      Intersection();
      /**
       * @brief Prepare ray intersection computations
       * @param ray
       * @return Computations
       */
      [[nodiscard]] Computations prepareComputations(Ray ray) const;
      /**
       * @brief Prepare ray intersection computations
       * @param ray
       * @return Computations
       */
      [[nodiscard]] Computations prepareComputations(Ray ray,
                                                     const Intersections &intersections) const;
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
      [[nodiscard]] std::optional<Intersection> hit() const;
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
      std::vector<std::shared_ptr<Shape>> objects;
      std::optional<PointLight> light;
      /**
       * Default constructor for a World
       */
      World();
      /**
       * Return true if the world contains no objects
       * @return true if the world contains no objects
       */
      [[nodiscard]] bool isEmpty() const;
      /**
       * Add an Object to the world
       * @param object target Object
       */
      void add(const std::shared_ptr<Shape> &object);
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
      Color shadeHit(const Computations &computations, int remaining);
      /**
       * Intersect the world with the given ray and return
       * the color at the resulting intersection
       * @param ray Ray to intersect with this World
       * @return Color at the resulting intersection
       */
      Color colorAt(Ray ray, int remaining);
      /**
       * Calculate the reflected color for a set of Computations
       * @param computations
       * @param remaining recursion limit
       * @return reflected color
       */
      Color reflectedColorAt(const Computations &computations, int remaining);
      /**
       * Calculated the refracted color for a set of Computations
       * @param computations
       * @param remaining recursion limit
       * @return refracted Color
       */
      Color refractedColorAt(const Computations &computations, int remaining);
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
      double pixelSize;
      double fieldOfView;
      double halfWidth;
      double halfHeight;
      Matrix transform;
      /**
       * Create a new camera
       * @param hSize horizontal size of canvas
       * @param vSize vertical size of canvas
       * @param fieldOfView camera angle
       */
      Camera(int hSize, int vSize, double fieldOfView);
      /**
       * Return a ray targeting the pixel at this position
       * @param x X position
       * @param y Y positionim
       * @return Ray
       */
      [[nodiscard]] Ray rayForPixel(int x, int y) const;
      /**
       * Render a world using this camera
       * @param world The World to render
       * @return a Canvas containing the rendered image
       */
      [[nodiscard]] Canvas render(World world) const;
    };
    /**
     * Calculate the lighting at a particular position on a material
     * using the Phong reflection model
     * @param shape Target object
     * @param light Light source
     * @param position Position we need the lighting for
     * @param eyeVector Eye vector
     * @param normalVector Normal vector
     * @param inShadow Whether or not the position is in shadow
     * @return The color for the target position
     */
    static Color lighting(const std::shared_ptr<Shape> &shape, PointLight light, Tuple position,
                          Tuple eyeVector, Tuple normalVector, bool inShadow);
  };
  /**
   * @brief A parser for OBJ files
   */
  class ObjParser {
  public:
    std::vector<RayTracerChallenge::Tuple> vertices = std::vector<RayTracerChallenge::Tuple>();
    std::vector<RayTracerChallenge::Tuple> normals = std::vector<RayTracerChallenge::Tuple>();
    std::shared_ptr<RayTracerChallenge::Shape> defaultGroup = RayTracerChallenge::Group::create();
    std::unordered_map<std::string, std::shared_ptr<RayTracerChallenge::Shape>> groups;
    ObjParser() { this->groups["Default"] = defaultGroup; }
    /**
     * Create an ObjParser from an input stream
     * @param input stream
     * @return ObjParser instance
     */
    static ObjParser parse(std::stringstream &stream);
    /**
     * Get the objects found by this parser
     * @return Objects from the input file parsed by this parser
     */
    std::shared_ptr<RayTracerChallenge::Shape> getObjects();
  };
}  // namespace raytracerchallenge
