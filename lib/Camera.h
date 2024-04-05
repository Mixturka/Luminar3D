#pragma once
#include <cmath>
#include <iostream>

struct CameraAperture {
  float width;
  float height;
};

class Camera {
 public:
  Camera();

  void SetCanvas();

  CameraAperture aperture_;
  float focal_length_;
  float near_clipping_;
  float far_clipping_;
  uint64_t image_width_;
  uint64_t image_height_;

  float angle_of_view_horizontal_;
  float angle_of_view_vertical_;
  float top_;
  float bottom_;
  float left_;
  float right_;
};
