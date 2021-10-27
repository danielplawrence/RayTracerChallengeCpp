#pragma once

#include <raytracerchallenge/base/Tuple.h>
#include <raytracerchallenge/shapes/Group.h>
#include <raytracerchallenge/shapes/Shape.h>

#include <memory>
#include <unordered_map>

namespace raytracerchallenge {
  /**
   * @brief A parser for OBJ files
   */
  class ObjParser {
  public:
    std::vector<Tuple> vertices = std::vector<Tuple>();
    std::vector<Tuple> normals = std::vector<Tuple>();
    std::shared_ptr<Shape> defaultGroup = Group::create();
    std::unordered_map<std::string, std::shared_ptr<Shape>> groups;
    ObjParser() { this->groups["Default"] = defaultGroup; }
    /**
     * Create an ObjParser from an input stream
     * @param input stream
     * @return ObjParser instance
     */
    static ObjParser parse(std::stringstream &stream);
    /**
     * Get the objects found by this parser
     * @return Objects from the input file parsed by this parser
     */
    std::shared_ptr<Shape> getObjects();
  };
}  // namespace raytracerchallenge