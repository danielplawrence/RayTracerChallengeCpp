#include <doctest/doctest.h>
#include <raytracerchallenge/patterns/CheckersPattern.h>
#include <raytracerchallenge/shapes/Plane.h>
#include <raytracerchallenge/shapes/Sphere.h>

#include <chrono>
#include <fstream>
#include <iostream>

#include "raytracerchallenge/base/Camera.h"
#include "raytracerchallenge/base/Tuple.h"
#include "raytracerchallenge/base/World.h"

using namespace raytracerchallenge;

long timeit(std::function<void()> func) {
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  func();
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  return duration;
}

std::shared_ptr<Shape> makeSphere() {
  auto s = Sphere::create();
  s->transform = s->transform.translated(rand() % 10, rand() % 10, rand() % 10);
  s->material->specular = 0.9;
  s->material->shininess = 300;
  s->material->reflective = 0.9;
  s->material->transparency = 0.9;
  s->material->refractiveIndex = 1.5;
  s->material->color = Color(static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
                             static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
                             static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
  return s;
}

void renderScene() {
  World world;

  Camera camera(600, 600, 1.5);
  auto from = Tuple::point(1.0, 1.0, -10.0);
  auto to = Tuple::point(2.0, 1.0, 0.0);
  auto up = Tuple::vector(0.0, 1.0, 0.0);
  camera.transform = Matrix::view(from, to, up);

  world.light = PointLight(Tuple::point(-9.0, 9.0, -9.0), Color(1.0, 1.0, 1.0));

  for (int i = 0; i < 10; ++i) {
    world.add(makeSphere());
  }

  auto plane = Plane::create();
  plane->material->color = {0.3, 0.3, 0.3};
  plane->material->specular = 0.9;
  plane->material->shininess = 300;
  plane->material->reflective = 0.9;
  plane->material->transparency = 0.9;
  plane->material->refractiveIndex = 0.5;

  auto image = camera.render(world);
  // std::ofstream out("test_image.ppm");
  // out << image.toPortablePixmap();
  // out.close();
}
TEST_CASE("Render speed") {
  auto dur = timeit(renderScene);
  std::cout << "Render complete in " << dur << " ms.";
  CHECK(dur < 20000);
}