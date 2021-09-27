#include <raytracerchallenge/raytracerchallenge.h>

#include <chrono>
#include <fstream>
#include <iostream>

using namespace raytracerchallenge;
auto main() -> int {
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  RayTracerChallenge::World world;
  world.light = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(0.0, 6.9, -5),
                                               RayTracerChallenge::Color(1.0, 1.0, 0.9));

  auto camera = RayTracerChallenge::Camera(3200, 1600, 0.785);
  auto from = RayTracerChallenge::Tuple::point(8.0, 12.0, -20.0);
  auto to = RayTracerChallenge::Tuple::point(0.0, 3.0, 0.0);
  auto up = RayTracerChallenge::Tuple::vector(0.0, 1.0, 0.0);
  camera.transform = RayTracerChallenge::Matrix::view(from, to, up);

  auto floor = RayTracerChallenge::Plane::create();
  auto floorPattern = RayTracerChallenge::CheckersPattern({0.0, 0.0, 0.0}, {0.25, 0.25, 0.25});
  floor->material.pattern = &floorPattern;
  floor->material.ambient = 0.25;
  floor->material.diffuse = 0.7;
  floor->material.specular = 0.9;
  floor->material.shininess = 300;
  floor->material.reflective = 0.1;

  auto wall = RayTracerChallenge::Cube::create();
  wall->transform = wall->transform.scaled(10.0, 10.0, 10.0);
  auto wallPattern
      = RayTracerChallenge::CheckersPattern({0.4863, 0.3765, 0.2941}, {0.3725, 0.2902, 0.2275});
  wallPattern.transform = wallPattern.transform.scaled(0.05, 20.0, 0.05);
  wall->material.pattern = &wallPattern;
  wall->material.ambient = 0.1;
  wall->material.diffuse = 0.7;
  wall->material.specular = 0.9;
  wall->material.shininess = 300;
  wall->material.reflective = 0.1;

  auto tableTop = RayTracerChallenge::Cube::create();
  tableTop->transform = tableTop->transform.scaled(3.0, 0.1, 2.0).translated(0.0, 3.1, 0.0);
  auto tableTopPattern
      = RayTracerChallenge::StripePattern({0.5529, 0.4235, 0.3255}, {0.6588, 0.5098, 0.4000});
  tableTopPattern.transform = tableTopPattern.transform.rotatedY(0.1).scaled(0.05, 0.05, 0.05);
  tableTop->material.pattern = &tableTopPattern;
  tableTop->material.ambient = 0.1;
  tableTop->material.diffuse = 0.7;
  tableTop->material.specular = 0.9;
  tableTop->material.shininess = 300;
  tableTop->material.reflective = 0.2;

  auto leg1 = RayTracerChallenge::Cube::create();
  leg1->transform = leg1->transform.scaled(0.1, 1.5, 0.1).translated(2.7, 1.5, -1.7);
  leg1->material.color = RayTracerChallenge::Color(0.5529, 0.4235, 0.3255);
  leg1->material.ambient = 0.2;
  leg1->material.diffuse = 0.7;

  auto leg2 = RayTracerChallenge::Cube::create();
  leg2->transform = leg2->transform.scaled(0.1, 1.5, 0.1).translated(2.7, 1.5, 1.7);
  leg2->material = leg1->material;

  auto leg3 = RayTracerChallenge::Cube::create();
  leg3->transform = leg3->transform.scaled(0.1, 1.5, 0.1).translated(-2.7, 1.5, -1.7);
  leg3->material = leg1->material;

  auto leg4 = RayTracerChallenge::Cube::create();
  leg4->transform = leg4->transform.scaled(0.1, 1.5, 0.1).translated(-2.7, 1.5, 1.7);
  leg4->material = leg1->material;

  auto glassCube = RayTracerChallenge::Cube::create();
  glassCube->transform
      = glassCube->transform.scaled(0.5, 0.5, 0.5).rotatedY(0.2).translated(0.0, 3.45001, 0.0);
  glassCube->material.color = RayTracerChallenge::Color(1.0, 1.0, 1.0);
  glassCube->material.ambient = 0.25;
  glassCube->material.diffuse = 0.0;
  glassCube->material.specular = 1.0;
  glassCube->material.shininess = 300;
  glassCube->material.reflective = 1.0;

  auto glassCube2 = RayTracerChallenge::Cube::create();
  glassCube2->transform
      = glassCube2->transform.scaled(0.5, 0.5, 0.5).rotatedY(-0.4).translated(1.0, 3.25001, 1.0);
  glassCube2->material.color = RayTracerChallenge::Color(1.0, 1.0, 1.0);
  glassCube2->material.ambient = 0.25;
  glassCube2->material.diffuse = 0.0;
  glassCube2->material.specular = 1.0;
  glassCube2->material.shininess = 300;
  glassCube2->material.reflective = 1.0;

  // world.add(floor);
  world.add(tableTop);
  world.add(leg1);
  world.add(leg2);
  world.add(leg3);
  world.add(leg4);
  world.add(glassCube);
  world.add(glassCube2);

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
