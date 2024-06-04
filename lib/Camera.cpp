#include "Camera.h"

Matrix4 camera_to_world = Matrix4(-0.95424, 0, 0.299041, 0, 0.0861242, 0.95763, 0.274823, 0, -0.28637, 0.288002, -0.913809, 0, -3.734612, 7.610426, -14.152769, 1);

Camera::Camera() {
  //this->SetTransform({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
}

void Camera::SetCanvas() {
  angle_of_view_horizontal_ =
      2 * atan((aperture_.width * 25.4 / 2) / focal_length_);
  right_ = tan(angle_of_view_horizontal_ / 2) * near_clipping_;


  angle_of_view_vertical_ =
      2 * atan((aperture_.height * 25.4 / 2) / focal_length_);
  top_ = tan(angle_of_view_vertical_ / 2) * near_clipping_;


  float xscale = 1; float yscale = 1;

  if ((this->aperture_.width / this->aperture_.height) > (800 / 600))
    xscale = (800 /  600) / (this->aperture_.width / this->aperture_.height); 
  else 
    yscale = (this->aperture_.width / this->aperture_.height) / (800 / 600);

  //right_ *= xscale;
  //top_ *= yscale;
  bottom_ = -top_;
  left_ = -right_;
}

void Camera::SetCameraMatrices() {
  Vec3<float> camera_orient;
  Vec3<float> z_vec(0, 0, 1);
  camera_orient = z_vec * this->camera_rotation_;
  float w = z_vec.x * this->camera_rotation_.matrix[0][3] +
            z_vec.y * this->camera_rotation_.matrix[1][3] +
            z_vec.z * this->camera_rotation_.matrix[2][3] +
            this->camera_rotation_.matrix[3][3];
  camera_orient.x /= w;
  camera_orient.y /= w;
  camera_orient.z /= w;

  Vec3<float> cam_pos = this->look_at_ + this->distance_to_target * camera_orient;

  camera_to_world = this->camera_rotation_;
  camera_to_world.matrix[3][0] = cam_pos.x;
  camera_to_world.matrix[3][1] = cam_pos.y;
  camera_to_world.matrix[3][2] = cam_pos.z;
}