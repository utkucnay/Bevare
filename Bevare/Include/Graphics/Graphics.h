#pragma once

#include "glm/fwd.hpp"

namespace bevare {

struct FGraphicsDesc {

};

class UGraphics {
 public:
  UGraphics();
  ~UGraphics();

 public:
  void RenderTriangle(glm::vec3& position);
};

}  // namespace bevare
