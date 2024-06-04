#pragma once
#include "Vec3.h"
#include "Matrix.h"
#include <cmath>

template <typename T>
class Quat {
  public:
    Quat() = default;
    Quat(T s, T i, T j, T k) : real(s), img_vec(i, j, k) {}
    Quat(T s, Vec3<T> vec) : real(s), img_vec(vec) {}

    Quat& SetAxisAngle(const Vec3<T>& axis, T radians);

    Matrix4 ToMat4() {
      return Matrix4(1 - 2 * (img_vec.y * img_vec.y + img_vec.z * img_vec.z), 
                     2 * (img_vec.x * img_vec.y + img_vec.z * real), 
                     2 * (img_vec.z * img_vec.x - img_vec.y * real),
                     0,
                     2 * (img_vec.x * img_vec.y - img_vec.z * real),
                     1 - 2 * (img_vec.z * img_vec.z + img_vec.x * img_vec.x),
                     2 * (img_vec.y * img_vec.z + img_vec.x * real),
                     0,
                     2 * (img_vec.z * img_vec.x + img_vec.y * real),
                     2 * (img_vec.y * img_vec.z - img_vec.x * real),
                     1 - 2 * (img_vec.y * img_vec.y + img_vec.x * img_vec.x),
                     0,
                     0,
                     0,
                     0,
                     1);
    }


  T real{1};
  Vec3<T> img_vec{0, 0, 0};
};

template <typename T>
inline Quat<T> operator*(const Quat<T>& q1, const Quat<T>& q2) {
  return Quat<T>(q1.real * q2.real - (q1.img_vec ^ q2.img_vec), q1.real * q2.img_vec + q1.img_vec * q2.real + q1.img_vec % q2.img_vec);
}

template <typename T>
Quat<T>& Quat<T>::SetAxisAngle(const Vec3<T>& axis, T radians) {
  img_vec = axis.Normalized() * std::sin(radians / 2);
  real = std::cos(radians / 2);

  return *this;
}