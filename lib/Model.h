#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Vec3.h"

class Mesh {
 public:
  Mesh(const std::string& model_path);

  std::string src_;
  std::vector<Vec3<float>> vertices_;
  //std::vector<std::pair<int, int>> edges_;
  std::vector<Vec3<int>> faces_;
};
