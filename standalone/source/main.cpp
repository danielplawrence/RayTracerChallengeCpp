#include <raytracerchallenge/raytracerchallenge.h>

#include <fstream>

using namespace raytracerchallenge;
auto main() -> int {
  RayTracerChallenge::World world;
  world.light
      = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(-10.0f, 10.0f, -10.0f),
                                       RayTracerChallenge::Color(1.0f, 1.0f, 1.0f));

  auto camera = RayTracerChallenge::Camera(1000, 500, float(M_PI) / 3.0f);
  auto from = RayTracerChallenge::Tuple::point(0.0f, 1.5f, -5.0f);
  auto to = RayTracerChallenge::Tuple::point(0.0f, 1.0f, 0.0f);
  auto up = RayTracerChallenge::Tuple::vector(0.0f, 1.0f, 0.0f);
  camera.transform = RayTracerChallenge::Matrix::view(from, to, up);

  RayTracerChallenge::Sphere floor;
  floor.transform = RayTracerChallenge::Matrix::scaling(10.0, 0.01, 10.0);
  floor.material.color = {1.0, 0.9, 0.9};
  floor.material.specular = 0.0;
  auto floorPattern = RayTracerChallenge::CheckersPattern(RayTracerChallenge::Color::WHITE,
                                                          RayTracerChallenge::Color::BLACK);
  floorPattern.transform = floorPattern.transform.scaled(0.125f, 0.125f, 1.0f);
  floor.material.pattern = &floorPattern;

  RayTracerChallenge::Sphere middle;
  middle.transform = RayTracerChallenge::Matrix::translation(-0.5f, 1.0f, 0.5f);
  auto pattern = RayTracerChallenge::GradientPattern({1.0, 0.0, 0.0}, {0.0, 0.0, 1.0});
  middle.material.pattern = &pattern;

  world.add(floor);
  world.add(middle);

  auto image = camera.render(world);

  std::ofstream out("output_lo_res.ppm");
  out << image.toPortablePixmap();
  out.close();
  return 0;
}
