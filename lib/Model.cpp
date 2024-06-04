#include "Model.h"

Mesh::Mesh(const std::string& model_path) {
  std::ifstream src(model_path);

  if (!src.is_open()) {
    std::cerr << "Unable to load model.\n";
  }

  std::string line;
  while (std::getline(src, line)) {
    if (line[0] == 'v' && line[1] == ' ') {
      line = line.substr(2);
      std::stringstream line_parser(line);
      Vec3<float> vertex;
      float x, y, z;

      line_parser >> vertex.x >> vertex.y >> vertex.z;
      this->vertices_.push_back(vertex);
    } else if (line[0] == 'f') {
      line = line.substr(2);
      std::stringstream line_parser(line);

      std::vector<std::vector<int>> v(3);

      for (int i = 0; i < 3; ++i) {
        std::string values;
        line_parser >> values;

        std::stringstream element_parser(values);
        int ind;
        std::vector<int> indices;

        while (element_parser >> ind) {
          indices.push_back(ind);
          if (element_parser.peek() == '/') {
            element_parser.ignore();
          } else {
            break;
          }
        }
        
        v[i] = indices;
        for (int index : indices) {
          std::cout << index << ' ';
        }
        std::cout << '\n';
      }

      Vec3<int> face(v[0][0], v[1][0], v[2][0]);
      faces_.push_back(face);
    }
  }
  std::cout << "FACES: \n";
  for (int i = 0; i < faces_.size(); ++i) {
    std::cout << faces_[i].x << ' ' << faces_[i].y << ' ' << faces_[i].z << '\n';
  }
}