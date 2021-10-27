#include <raytracerchallenge/base/Camera.h>
#include <raytracerchallenge/base/Canvas.h>
#include <raytracerchallenge/base/Ray.h>
#include <raytracerchallenge/base/World.h>
#include <raytracerchallenge/parallel/Parallel.h>

namespace raytracerchallenge {
  Camera::Camera(int hSize, int vSize, double fieldOfView) {
    this->hSize = hSize;
    this->vSize = vSize;
    this->fieldOfView = fieldOfView;
    this->transform = Matrix::identity(4);
    auto halfView = tan(this->fieldOfView / 2.0);
    auto aspect = double(this->hSize) / double(this->vSize);
    if (aspect >= 1) {
      halfWidth = halfView;
      halfHeight = halfView / aspect;
    } else {
      halfWidth = halfView * aspect;
      halfHeight = halfView;
    }
    pixelSize = (halfWidth * 2.0) / double(hSize);
  }
  Ray Camera::rayForPixel(int x, int y) const {
    auto xOffset = (x + 0.5) * pixelSize;
    auto yOffset = (y + 0.5) * pixelSize;
    auto worldX = double(halfWidth - xOffset);
    auto worldY = double(halfHeight - yOffset);
    auto pixel = transform.inverse() * Tuple::point(worldX, worldY, -1.0);
    auto origin = transform.inverse() * Tuple::point(0.0, 0.0, 0.0);
    auto direction = (pixel - origin).normalize();
    return {origin, direction};
  }
  Canvas Camera::render(World world) const {
    auto image = Canvas(hSize, vSize);
    parallelFor(vSize, [this, &world, &image](int s, int e) {
      for (int y = s; y < e; y++) {
        for (int x = 0; x < hSize; x++) {
          auto ray = rayForPixel(x, y);
          auto color = world.colorAt(ray, 4);
          image.writePixel(x, y, color);
        }
      }
    });
    return image;
  }
}  // namespace raytracerchallenge