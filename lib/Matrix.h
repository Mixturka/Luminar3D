#pragma once
#include <iostream>

struct Matrix4 {
  float** matrix;

  Matrix4(float c00, float c01, float c02, float c03, float c10, float c11,
          float c12, float c13, float c20, float c21, float c22, float c23,
          float c30, float c31, float c32, float c33);

  Matrix4(const Matrix4& other);

  Matrix4& operator=(const Matrix4& other);

  Matrix4 Inverse();

  ~Matrix4();
};
