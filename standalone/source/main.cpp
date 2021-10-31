#include <chrono>
#include <fstream>
#include <iostream>

#include "raytracerchallenge/base/Camera.h"
#include "raytracerchallenge/base/Tuple.h"
#include "raytracerchallenge/base/World.h"
#include "raytracerchallenge/io/ObjParser.h"

using namespace raytracerchallenge;

auto main() -> int {
  std::cout << "Starting rendering" << std::endl;
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

  World world;

  Camera camera(600, 250, 1.047);
  auto from = Tuple::point(0.0, 0.0, -15.0);
  auto to = Tuple::point(0.5, 0.0, 0.0);
  auto up = Tuple::vector(0.0, 1.0, 0.0);
  camera.transform = Matrix::view(from, to, up);

  world.light = PointLight(Tuple::point(-9.0, 9.0, -9.0), Color(0.8, 0.8, 0.8));

  std::cout << "Loading file" << std::endl;
  std::ifstream file("pumpkin2.obj");
  if (file) {
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::cout << "Read the file" << std::endl;
    auto parser = ObjParser::parse(buffer);
    auto objects = parser.getObjects();
    objects->divide(1);

    objects->transform = objects->transform.scaled(4, 4, 4);
    objects->transform = objects->transform.translated(0.0, -2.0, 0.0);
    objects->transform = objects->transform.rotatedX(-M_PI / 4);

    objects->material->color = Color(1.0, 0.6, 0.0);

    std::cout << "Parsed the file" << std::endl;
    std::cout << "Found " << std::dynamic_pointer_cast<Group>(objects)->objects.size() << " objects"
              << std::endl;
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
