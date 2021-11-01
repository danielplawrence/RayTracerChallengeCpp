#include <raytracerchallenge/shapes/Cube.h>

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

  Camera camera(1200, 1200, 1.047);
  auto from = Tuple::point(1.0, 1.0, -10.0);
  auto to = Tuple::point(1.0, 0.0, 0.0);
  auto up = Tuple::vector(0.0, 1.0, 0.0);
  camera.transform = Matrix::view(from, to, up);

  world.light = PointLight(Tuple::point(0.75, 3.0, -8.0), Color(0.8, 0.8, 0.8));
  std::cout << "Loading file" << std::endl;
  std::ifstream file("pumpkin2.obj");
  auto bg = Cube::create();
  bg->material->color = {1.0, 1.0, 1.0};
  bg->transform = bg->transform.scaled(16, 16, 16).translated(0.0, 12.0, 0.0);
  bg->material->diffuse = 0.5;
  bg->material->specular = 0.0;
  if (file) {
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::cout << "Read the file" << std::endl;
    auto parser = ObjParser::parse(buffer);
    auto objects = parser.getObjects();

    auto stemBase = std::dynamic_pointer_cast<Group>(parser.groups["StemBase"]);
    auto stemBaseElements = stemBase->objects;
    auto stem = std::dynamic_pointer_cast<Group>(parser.groups["Stem"]);
    auto stemElements = stem->objects;
    auto stemTip = std::dynamic_pointer_cast<Group>(parser.groups["StemTip"]);
    auto stemTipElements = stemTip->objects;

    objects->divide(1);

    objects->transform = objects->transform.scaled(4, 4, 4);
    objects->transform = objects->transform.rotatedX(-M_PI / 8);
    objects->transform = objects->transform.translated(1.0, -3.0, 0.0);

    objects->material->color = Color(1.5, 0.6, 0.0);
    objects->material->shininess = 50.0;
    objects->material->specular = 0.25;
    objects->material->castShadow = false;

    for (const auto& elem : stemBaseElements) {
      elem->material = std::shared_ptr<Material>(new Material());
      elem->material->shininess = 50.0;
      elem->material->specular = 0.25;
      elem->material->castShadow = false;
      elem->material->color = {0.5, 0.5, 0.3};
    }

    for (const auto& elem : stemElements) {
      elem->material = std::shared_ptr<Material>(new Material());
      elem->material->shininess = 50.0;
      elem->material->specular = 0.25;
      elem->material->castShadow = false;
      elem->material->color = {0.5, 0.5, 0.3};
    }

    for (const auto& elem : stemTipElements) {
      elem->material = std::shared_ptr<Material>(new Material());
      elem->material->shininess = 50.0;
      elem->material->specular = 0.25;
      elem->material->castShadow = false;
      elem->material->color = {0.5, 0.5, 0.3};
    }

    std::cout << "Parsed the file" << std::endl;
    std::cout << "Found " << std::dynamic_pointer_cast<Group>(objects)->objects.size() << " objects"
              << std::endl;
    world.add(objects);
  }
  world.add(bg);
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
