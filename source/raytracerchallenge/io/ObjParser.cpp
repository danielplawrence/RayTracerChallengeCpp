#include <raytracerchallenge/io/ObjParser.h>
#include <raytracerchallenge/shapes/SmoothTriangle.h>
#include <raytracerchallenge/shapes/Triangle.h>

#include <iostream>
#include <regex>

namespace raytracerchallenge {
  std::vector<std::shared_ptr<Shape>> fanTriangulation(std::vector<Tuple> vertices) {
    auto triangles = std::vector<std::shared_ptr<Shape>>();
    for (int index = 2; index < int(vertices.size()) - 1; ++index) {
      auto tri = Triangle::create(vertices[1], vertices[index], vertices[index + 1]);
      triangles.push_back(tri);
    }
    return triangles;
  }
  std::vector<std::shared_ptr<Shape>> fanTriangulation(std::vector<Tuple> vertices,
                                                       std::vector<Tuple> normals) {
    auto triangles = std::vector<std::shared_ptr<Shape>>();
    for (int index = 2; index < int(vertices.size()) - 1; ++index) {
      auto tri = SmoothTriangle::create(vertices[1], vertices[index], vertices[index + 1],
                                        normals[1], normals[index], normals[index + 1]);
      triangles.push_back(tri);
    }
    return triangles;
  }
  ObjParser ObjParser::parse(std::stringstream &lines) {
    auto parser = ObjParser();
    parser.vertices.emplace_back();
    parser.normals.emplace_back();
    std::string oneLine;
    std::smatch subMatch;
    std::regex vertexLinePattern
        = std::regex(R"((v) (-*\d(\.*\d+)*) (-*\d(\.*\d+)*) (-*\d(\.*\d+)*))");
    std::regex vertexNormalLinePattern
        = std::regex(R"((vn) (-*\d(\.*\d+)*) (-*\d(\.*\d+)*) (-*\d(\.*\d+)*))");
    std::regex faceLinePattern = std::regex(R"((f) ((\d *){3,}))");
    std::regex faceLinePatternWithNormals = std::regex(R"((f) (\d+\/\d*\/\d+ *){3,})");
    std::regex groupLinePattern = std::regex(R"((g) ([a-zA-Z0-9]+))");
    std::string lastMentionedGroup;
    while (getline(lines, oneLine)) {
      if (std::regex_match(oneLine, subMatch, groupLinePattern)) {
        lastMentionedGroup = subMatch.str(2);
      } else if (std::regex_match(oneLine, subMatch, vertexLinePattern)) {
        auto x = std::stod(subMatch.str(2));
        auto y = std::stod(subMatch.str(4));
        auto z = std::stod(subMatch.str(6));
        parser.vertices.push_back(Tuple::point(x, y, z));
      } else if (std::regex_match(oneLine, subMatch, vertexNormalLinePattern)) {
        auto n1 = std::stod(subMatch.str(2));
        auto n2 = std::stod(subMatch.str(4));
        auto n3 = std::stod(subMatch.str(6));
        parser.normals.push_back(Tuple::vector(n1, n2, n3));
      } else if (std::regex_match(oneLine, subMatch, faceLinePattern)) {
        auto bounds = BoundingBox();
        for (auto vert : parser.vertices) {
          bounds.add(vert);
        }
        auto sx = bounds.max.x - bounds.min.x;
        auto sy = bounds.max.y - bounds.min.y;
        auto sz = bounds.max.z - bounds.min.z;
        auto scale = std::max({sx, sy, sz}) / 2;
        for (auto vert : parser.vertices) {
          vert.x = (vert.x - (bounds.min.x + sx / 2)) / scale;
          vert.y = (vert.y - (bounds.min.y + sy / 2)) / scale;
          vert.z = (vert.z - (bounds.min.z + sz / 2)) / scale;
        }
        auto indices = subMatch.str(2);
        std::stringstream ss(indices);
        std::string s;
        std::vector<Tuple> targetVertices;
        targetVertices.emplace_back();
        while (std::getline(ss, s, ' ')) {
          targetVertices.push_back(parser.vertices[std::stoi(s)]);
        }
        auto triangles = fanTriangulation(targetVertices);
        for (const auto &triangle : triangles) {
          if (lastMentionedGroup.empty()) {
            std::dynamic_pointer_cast<Group>(parser.defaultGroup)->add(triangle);
          } else {
            if (!parser.groups.count(lastMentionedGroup)) {
              parser.groups[lastMentionedGroup] = Group::create();
            }
            std::dynamic_pointer_cast<Group>(parser.groups[lastMentionedGroup])->add(triangle);
          }
        }
      } else if (std::regex_match(oneLine, faceLinePatternWithNormals)) {
        std::stringstream ss(oneLine);
        std::string s;
        std::vector<Tuple> targetVertices;
        std::vector<Tuple> targetNormals;
        targetVertices.emplace_back();
        targetNormals.emplace_back();
        while (std::getline(ss, s, ' ')) {
          if (s == "f") {
            continue;
          }
          auto firstSlashIndex = s.find('/');
          auto lastSlashIndex = s.rfind('/');
          targetVertices.push_back(parser.vertices[std::stoi(s.substr(0, firstSlashIndex))]);
          targetNormals.push_back(
              parser.normals[stoi(s.substr(lastSlashIndex + 1, s.size() - lastSlashIndex - 1))]);
        }
        auto triangles = fanTriangulation(targetVertices, targetNormals);
        for (const auto &triangle : triangles) {
          if (lastMentionedGroup.empty()) {
            std::dynamic_pointer_cast<Group>(parser.defaultGroup)->add(triangle);
          } else {
            if (!parser.groups.count(lastMentionedGroup)) {
              parser.groups[lastMentionedGroup] = Group::create();
            }
            std::dynamic_pointer_cast<Group>(parser.groups[lastMentionedGroup])->add(triangle);
          }
        }
      } else {
        std::cout << "Skipped line: " << oneLine << std::endl;
      }
    }
    return parser;
  }
  std::shared_ptr<Shape> ObjParser::getObjects() {
    auto group = new Group();
    for (auto const &item : this->groups) {
      group->add(item.second);
    }
    return group->sharedPtr;
  }
}  // namespace raytracerchallenge