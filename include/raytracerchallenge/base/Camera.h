#pragma once

#include <raytracerchallenge/base/Canvas.h>
#include <raytracerchallenge/base/Matrix.h>
#include <raytracerchallenge/base/Ray.h>
#include <raytracerchallenge/base/World.h>

namespace raytracerchallenge {
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
    [[nodiscard]] Ray rayForPixel(int x, int y);
    /**
     * Render a world using this camera
     * @param world The World to render
     * @return a Canvas containing the rendered image
     */
    [[nodiscard]] Canvas render(World world);
  };
}  // namespace raytracerchallenge