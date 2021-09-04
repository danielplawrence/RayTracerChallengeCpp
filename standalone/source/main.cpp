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
  RayTracerChallenge::Sphere shape;
  shape.material.color = RayTracerChallenge::Color(1.0f, 0.2f, 1.0f);
  auto lightPosition = RayTracerChallenge::Tuple::point(-10.0f, 10.0f, -10.0f);
  auto lightColor = RayTracerChallenge::Color(1.0f, 1.0f, 1.0f);
  RayTracerChallenge::PointLight light(lightPosition, lightColor);
  for (int y = 0; y < canvasPixels; y++) {
    float worldY = half - pixelSize * float(y);
    for (int x = 0; x < canvasPixels; x++) {
      float worldX = -half + pixelSize * float(x);
      RayTracerChallenge::Tuple position = RayTracerChallenge::Tuple::point(worldX, worldY, wallZ);
      RayTracerChallenge::Ray ray(rayOrigin, (position - rayOrigin).normalize());
      RayTracerChallenge::Intersections xs = shape.intersect(ray);
      if (xs.hit().has_value()) {
        auto point = ray.position(xs.hit()->t);
        auto normal = xs.hit()->object.normalAt(point);
        auto eye = -ray.direction;
        auto color
            = RayTracerChallenge::lighting(xs.hit()->object.material, light, point, eye, normal);
        canvas.writePixel(x, y, color);
      }
    }
  }
  std::ofstream out("output.ppm");
  out << canvas.toPortablePixmap();
  out.close();
  return 0;
}
