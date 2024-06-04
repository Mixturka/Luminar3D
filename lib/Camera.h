#pragma once
#include <cmath>
#include <iostream>
#include "Math/Matrix.h"
#include "Math/Vec2.h"
#include "Math/Vec3.h"

struct CameraAperture {
  float width;
  float height;
};

extern Matrix4 camera_to_world;

class Camera {
 public:
  Camera();

  void SetCanvas();

  void SetTransform(const Matrix4& new_transform) {
    //this->transform_ = new_transform;
  }

  void SetCameraMatrices();

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
  float kRotateAmplitude = 0.01;
  float kPanAmplitude = 0.02;
  float kScrollAmplitude = 1;

  float  distance_to_target;

  Vec2<int> mouse_pos_;
  Vec2<float> rotation_params_ = Vec2<float>(0, 0);
  Vec3<float> look_at_ = Vec3<float>(0, 0, 0);

  Matrix4 camera_rotation_ = Matrix4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  enum class CameraMoveType: uint8_t {
    NONE,
    TUMBLE,
    TRACK,
    DOLLY,
  };

  CameraMoveType move_type_;
};
