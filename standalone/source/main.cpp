#include <raytracerchallenge/raytracerchallenge.h>

#include <fstream>

using namespace raytracerchallenge;
auto main() -> int {
  RayTracerChallenge::World world;
  world.light
      = RayTracerChallenge::PointLight(RayTracerChallenge::Tuple::point(-10.0f, 10.0f, -10.0f),
                                       RayTracerChallenge::Color(1.0f, 1.0f, 1.0f));

  auto camera = RayTracerChallenge::Camera(50, 25, float(M_PI) / 3.0f);
  auto from = RayTracerChallenge::Tuple::point(0.0f, 1.5f, -5.0f);
  auto to = RayTracerChallenge::Tuple::point(0.0f, 1.0f, 0.0f);
  auto up = RayTracerChallenge::Tuple::vector(0.0f, 1.0f, 0.0f);
  camera.transform = RayTracerChallenge::Matrix::view(from, to, up);

  RayTracerChallenge::Sphere floor;
  floor.transform = RayTracerChallenge::Matrix::scaling(10.0f, 0.01f, 10.0f);
  floor.material.color = {1.0f, 0.9f, 0.9f};
  floor.material.specular = 0.0f;

  RayTracerChallenge::Sphere leftWall;
  leftWall.transform = RayTracerChallenge::Matrix::translation(0.0f, 0.0f, 5.0f)
                       * RayTracerChallenge::Matrix::rotationY(-float(M_PI) / 4.0f)
                       * RayTracerChallenge::Matrix::rotationX(float(M_PI) / 2.0f)
                       * RayTracerChallenge::Matrix::scaling(10.0f, 0.01f, 10.0f);
  leftWall.material.color = {1.0f, 0.9f, 0.9f};
  leftWall.material.specular = 0.0f;

  RayTracerChallenge::Sphere rightWall;
  rightWall.transform = RayTracerChallenge::Matrix::translation(0.0f, 0.0f, 5.0f)
                        * RayTracerChallenge::Matrix::rotationY(float(M_PI) / 4.0f)
                        * RayTracerChallenge::Matrix::rotationX(float(M_PI) / 2.0f)
                        * RayTracerChallenge::Matrix::scaling(10.0f, 0.01f, 10.0f);
  rightWall.material.color = {1.0f, 0.9f, 0.9f};
  rightWall.material.specular = 0.0f;

  RayTracerChallenge::Sphere middle;
  middle.transform = RayTracerChallenge::Matrix::translation(-0.5f, 1.0f, 0.5f);
  middle.material.color = {0.1f, 1.0f, 0.5f};
  middle.material.specular = 0.3f;
  middle.material.diffuse = 0.7f;

  RayTracerChallenge::Sphere right;
  right.transform = RayTracerChallenge::Matrix::translation(1.5f, 0.5f, -0.5f)
                    * RayTracerChallenge::Matrix::scaling(0.5f, 0.5f, 0.5f);
  right.material.color = {0.5f, 1.0f, 0.1f};
  right.material.specular = 0.3f;
  right.material.diffuse = 0.7f;

  RayTracerChallenge::Sphere left;
  left.transform = RayTracerChallenge::Matrix::translation(-1.5f, 0.33f, -0.75f)
                   * RayTracerChallenge::Matrix::scaling(0.33f, 0.33f, 0.33f);
  left.material.color = {1.0f, 0.8f, 0.1f};
  left.material.specular = 0.3f;
  left.material.diffuse = 0.7f;

  world.add(floor);
  world.add(leftWall);
  world.add(rightWall);
  world.add(middle);
  world.add(right);
  world.add(left);

  auto image = camera.render(world);

  std::ofstream out("output_lo_res.ppm");
  out << image.toPortablePixmap();
  out.close();
  return 0;
}
