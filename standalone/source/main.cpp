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

  RayTracerChallenge::Camera camera(300, 225, 1.047);
  auto from = RayTracerChallenge::Tuple::point(0.0, 0.0, -15.0);
  auto to = RayTracerChallenge::Tuple::point(0.5, 0.0, 0.0);
  auto up = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
  camera.transform = RayTracerChallenge::Matrix::view(from, to, up);

  world.light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(-9.0, 9.0, -9.0),
                                               RayTracerChallenge::Color(0.8, 0.8, 0.8));

  std::cout << "Loading file" << std::endl;
  std::ifstream file("teapot.obj");
  if (file) {
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::cout << "Read the file" << std::endl;
    auto parser = ObjParser::parse(buffer);
    auto objects = parser.getObjects();
    objects->transform = objects->transform.rotatedX(-M_PI / 2.0);
    objects->transform = objects->transform.rotatedY(M_PI / 2.0);
    std::cout << "Parsed the file" << std::endl;
    std::cout << "Found "
              << std::dynamic_pointer_cast<RayTracerChallenge::Group>(objects)->objects.size()
              << " objects" << std::endl;
    ;
    world.add(objects);
  }

  std::cout << "Rendering objects" << std::endl;
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
