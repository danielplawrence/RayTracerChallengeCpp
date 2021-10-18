#include <raytracerchallenge/raytracerchallenge.h>

#include <chrono>
#include <fstream>
#include <iostream>

using namespace raytracerchallenge;

std::shared_ptr<RayTracerChallenge::Shape> makeSphere() {
  auto s = RayTracerChallenge::Sphere::create();
  s->transform = s->transform.translated((rand() % 5) * 5, (rand() % 3) * 5, (rand() % 5) * 5);
  s->material.specular = 0.9;
  s->material.shininess = 300;
  s->material.reflective = 0.9;
  s->material.transparency = 0.9;
  s->material.refractiveIndex = 1.5;
  s->material.color
      = RayTracerChallenge::Color(static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
                                  static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
                                  static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
  return s;
}

auto main() -> int {
  std::cout << "Starting rendering" << std::endl;
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

  RayTracerChallenge::World world;

  RayTracerChallenge::Camera camera(400, 400, 1.047);
  auto from = RayTracerChallenge::Tuple::point(3.0, 3.0, -3.0);
  auto to = RayTracerChallenge::Tuple::point(0.0, 1.0, 0.0);
  auto up = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
  camera.transform = RayTracerChallenge::Matrix::view(from, to, up);

  world.light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(5.0, 10.0, -5.0),
                                               RayTracerChallenge::Color(1.0, 1.0, 1.0));

  auto cube1 = RayTracerChallenge::Cylinder::create(1.0, 2.0, true);
  cube1->transform = cube1->transform.rotatedX(M_PI / 2).rotatedZ(M_PI / 2).rotatedY(M_PI / 8);

  auto cube2 = RayTracerChallenge::Cylinder::create(1.0, 2.0);
  cube2->transform = cube2->transform.rotatedX(M_PI / 2).translated(0.0, 0.0, 5.0);

  world.add(cube1);
  world.add(cube2);
  // world.add(plane);
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
