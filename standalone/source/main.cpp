#include <raytracerchallenge/raytracerchallenge.h>

#include <fstream>

using namespace raytracerchallenge;
auto main() -> int {
  RayTracerChallenge::Tuple rayOrigin = RayTracerChallenge::Tuple::point(0.0f, 0.0f, -5.0f);
  float wallZ = 10.0f;
  float wallSize = 7.0f;
  int canvasPixels = 100;
  float pixelSize = wallSize / float(canvasPixels);
  float half = wallSize / 2;
  RayTracerChallenge::Canvas canvas(canvasPixels, canvasPixels);
  RayTracerChallenge::Color color(1.0f, 0.0f, 0.0f);
  RayTracerChallenge::Sphere shape;
  shape.transform = RayTracerChallenge::Matrix::scaling(1.0f, 0.5f, 1.0f);
  for (int y = 0; y < canvasPixels; y++) {
    float worldY = half - pixelSize * float(y);
    for (int x = 0; x < canvasPixels; x++) {
      float worldX = -half + pixelSize * float(x);
      RayTracerChallenge::Tuple position = RayTracerChallenge::Tuple::point(worldX, worldY, wallZ);
      RayTracerChallenge::Ray ray(rayOrigin, (position - rayOrigin).normalize());
      RayTracerChallenge::Intersections xs = shape.intersect(ray);
      if (xs.hit().has_value()) {
        canvas.writePixel(x, y, color);
      }
    }
  }
  std::ofstream out("output.ppm");
  out << canvas.toPortablePixmap();
  out.close();
  return 0;
}
