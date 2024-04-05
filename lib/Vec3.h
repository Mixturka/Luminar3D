#pragma once
#include <iostream>

#include "Matrix.h"

template <typename T>
struct Vec3 {
  T x;
  T y;
  T z;

  Vec3(T x_, T y_, T z_);
  Vec3() = default;
  Vec3(const Vec3& other);
  Vec3(Vec3&& other);

  Vec3 operator*(const Matrix4& m) const;

  Vec3& operator=(Vec3& other);

  Vec3& operator=(Vec3&& other);
};

template <typename T>
Vec3<T>::Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

template <typename T>
Vec3<T>& Vec3<T>::operator=(Vec3<T>& other) {
  this->x = other.x;
  this->y = other.y;
  this->z = other.z;

  return *this;
}

template <typename T>
Vec3<T>& Vec3<T>::operator=(Vec3<T>&& other) {
  if (this != &other) {
    x = std::move(other.x);
    y = std::move(other.y);
    z = std::move(other.z);
  }
  return *this;
}

template <typename T>
Vec3<T> Vec3<T>::operator*(const Matrix4& m) const {
  float new_x = x * m.matrix[0][0] + y * m.matrix[1][0] + z * m.matrix[2][0] +
                m.matrix[3][0];
  float new_y = x * m.matrix[0][1] + y * m.matrix[1][1] + z * m.matrix[2][1] +
                m.matrix[3][1];
  float new_z = x * m.matrix[0][2] + y * m.matrix[1][2] + z * m.matrix[2][2] +
                m.matrix[3][2];

  return Vec3<T>(new_x, new_y, new_z);
}

template <typename T>
Vec3<T>::Vec3(const Vec3<T>& other) : x(other.x), y(other.y), z(other.z) {}

template <typename T>
Vec3<T>::Vec3(Vec3<T>&& other) : x(std::move(other.x)), y(std::move(other.y)), z(std::move(other.z)) {}