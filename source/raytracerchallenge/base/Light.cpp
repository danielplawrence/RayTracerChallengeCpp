
#include <raytracerchallenge/base/Light.h>

namespace raytracerchallenge {
  Color lighting(const std::shared_ptr<Shape> &shape, PointLight light, Tuple position,
                 Tuple eyeVector, Tuple normalVector, bool inShadow) {
    Color diffuse;
    Color specular;
    Color ambient;
    Color color;
    if (shape->material.pattern != nullptr) {
      color = shape->material.pattern->colorAt(shape, position);
    } else {
      color = shape->material.color;
    }
    auto effectiveColor = color * light.intensity;
    auto lightVector = (light.position - position).normalize();
    ambient = effectiveColor * shape->material.ambient;
    if (inShadow) {
      return ambient;
    }
    auto lightDotNormal = lightVector.dot(normalVector);
    if (lightDotNormal < 0.0) {
      diffuse = Color(0.0, 0.0, 0.0);
      specular = Color(0.0, 0.0, 0.0);
    } else {
      diffuse = effectiveColor * shape->material.diffuse * lightDotNormal;
      auto reflectVector = (-lightVector).reflect(normalVector);
      auto reflectDotEye = reflectVector.dot(eyeVector);
      if (reflectDotEye <= 0.0) {
        specular = Color(0.0, 0.0, 0.0);
      } else {
        auto factor = pow(reflectDotEye, shape->material.shininess);
        specular = light.intensity * shape->material.specular * factor;
      }
    }
    return ambient + diffuse + specular;
  }
}  // namespace raytracerchallenge