#include <raytracerchallenge/raytracerchallenge.h>

#include <fstream>
#include <iostream>
#include <chrono>

using namespace raytracerchallenge;
auto main() -> int {
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  RayTracerChallenge::World world;
  world.light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(2.0, 10.0, -5),
                                               RayTracerChallenge::Color(0.9, 0.9, 0.9));

  auto camera = RayTracerChallenge::Camera(200, 200, 0.45);
  auto from = RayTracerChallenge::Tuple::point(0.0, 0.0, -5.0);
  auto to = RayTracerChallenge::Tuple::point(0.0, 0.0, 0.0);
  auto up = RayTracerChallenge::Tuple::vector(0.0, 0.5, 0.0);
  camera.transform = RayTracerChallenge::Matrix::view(from, to, up);

  auto wall = RayTracerChallenge::Plane::create();
  wall->transform = wall->transform.rotatedX(1.5708).translated(0.0, 0.0, 10.0);
  auto pattern = RayTracerChallenge::CheckersPattern({0.15, 0.15, 0.15}, {0.85, 0.85, 0.85});
  wall->material.pattern = &pattern;
  wall->material.specular = 0.0;
  wall->material.diffuse = 0.2;
  wall->material.ambient = 0.8;
  wall->material.reflective = 0.9;
  wall->material.shininess = 300.0;
  wall->material.refractiveIndex = 1.5;

  auto glassBall = RayTracerChallenge::Sphere::create();
  glassBall->material.color = RayTracerChallenge::Color(0.85, 0.0, 0.0);
  glassBall->material.ambient = 0.9;
  glassBall->material.diffuse = 0.9;
  glassBall->material.specular = 0.9;
  glassBall->material.shininess = 300.0;
  glassBall->material.reflective = 0.9;
  glassBall->material.transparency = 0.9;
  glassBall->material.refractiveIndex = 1.5;

  world.add(wall);
  world.add(glassBall);

  auto image = camera.render(world);

  std::ofstream out("output_lo_res_test.ppm");
  out << image.toPortablePixmap();
  out.close();
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
  std::cout << "Processed " << camera.hSize * camera.vSize << " pixels in " << duration
            << " seconds.";
  return 0;
}
