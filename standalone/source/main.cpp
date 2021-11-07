#include <raytracerchallenge/shapes/Cube.h>
#include <raytracerchallenge/shapes/Plane.h>

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

  Camera camera(800, 800, 1.047);
  auto from = Tuple::point(1.0, 1.0, -10.0);
  auto to = Tuple::point(1.0, 0.0, 0.0);
  auto up = Tuple::vector(0.0, 1.0, 0.0);
  camera.transform = Matrix::view(from, to, up);

  world.light = PointLight(Tuple::point(0.75, 3.0, -8.0), Color(0.8, 0.8, 0.8));
  std::cout << "Loading file" << std::endl;
  std::ifstream file("dragon.obj");
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  std::cout << "Read the file" << std::endl;
  auto parser = ObjParser::parse(buffer);
  auto objects = parser.getObjects();
  objects->divide(1);

  std::cout << "Parsed the file" << std::endl;
  std::cout << "Found " << std::dynamic_pointer_cast<Group>(objects)->objects.size() << " objects"
            << std::endl;
  std::cout << "Rendering objects" << std::endl;
  objects->transform = objects->transform.rotatedY(M_PI / 1.25);
  objects->material->color = {1.0, 0.0, 0.0};
  auto plane = Plane::create();
  plane->material->reflective = 0.9;
  plane->material->color = {0.0, 0.0, 0.0};
  world.add(objects);
  world.add(plane);
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
