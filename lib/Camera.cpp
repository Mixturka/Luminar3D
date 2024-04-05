#include "Camera.h"

Camera::Camera() {}

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
