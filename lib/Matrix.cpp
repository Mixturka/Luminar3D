#include "Matrix.h"

Matrix4::Matrix4(float c00, float c01, float c02, float c03, float c10,
                 float c11, float c12, float c13, float c20, float c21,
                 float c22, float c23, float c30, float c31, float c32,
                 float c33) {
  matrix = new float*[4];

  for (int i = 0; i < 4; ++i) {
    matrix[i] = new float[4];
  }

  matrix[0][0] = c00;
  matrix[0][1] = c01;
  matrix[0][2] = c02;
  matrix[0][3] = c03;

  matrix[1][0] = c10;
  matrix[1][1] = c11;
  matrix[1][2] = c12;
  matrix[1][3] = c13;

  matrix[2][0] = c20;
  matrix[2][1] = c21;
  matrix[2][2] = c22;
  matrix[2][3] = c23;

  matrix[3][0] = c30;
  matrix[3][1] = c31;
  matrix[3][2] = c32;
  matrix[3][3] = c33;
}

Matrix4::Matrix4(const Matrix4& other) {
  matrix = new float*[4];
  for (int i = 0; i < 4; ++i) {
    matrix[i] = new float[4];
    for (int j = 0; j < 4; ++j) {
      matrix[i][j] = other.matrix[i][j];
    }
  }
}

Matrix4& Matrix4::operator=(const Matrix4& other) {
  if (this == &other) {
    return *this;
  }

  for (int i = 0; i < 4; ++i) {
    delete[] matrix[i];
  }
  delete[] matrix;

  matrix = new float*[4];
  for (int i = 0; i < 4; ++i) {
    matrix[i] = new float[4];
    for (int j = 0; j < 4; ++j) {
      matrix[i][j] = other.matrix[i][j];
    }
  }

  return *this;
}

Matrix4::~Matrix4() {
  std::cout << "~";
  for (int i = 0; i < 4; ++i) {
    delete[] matrix[i];
  }

  delete[] matrix;
}

Matrix4 Matrix4::Inverse() {
  Matrix4 result(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f,
                 0.f, 0.f, 0.f, 1.f);

  for (int i = 0; i < 3; ++i) {
    int pivot = i;

    float pivot_size = this->matrix[i][i];

    if (pivot_size < 0) pivot_size = -pivot_size;

    for (int j = i + 1; j < 4; ++j) {
      float temp = this->matrix[i][j];

      if (temp < 0) temp = -temp;

      if (temp > pivot_size) {
        pivot = j;
        pivot_size = temp;
      }
    }

    if (pivot_size == 0) {
      return Matrix4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }

    if (pivot != i) {
      for (int j = 0; j < 4; ++j) {
        std::swap(this->matrix[i][j], this->matrix[pivot][j]);
        std::swap(result.matrix[i][j], result.matrix[pivot][j]);
      }
    }

    for (int row = i + 1; row < 4; ++row) {
      float k = this->matrix[row][i] / this->matrix[i][i];

      for (int j = 0; j < 4; ++j) {
        this->matrix[row][j] -= k * this->matrix[i][j];
        result.matrix[row][j] -= k * result.matrix[i][j];
      }

      this->matrix[row][i] = 0.f;
    }
  }

  for (int row = 3; row >= 0; --row) {
    float div = this->matrix[row][row];

    for (int column = 0; column < 4; ++column) {
      this->matrix[row][column] = this->matrix[row][column] / div;
      result.matrix[row][column] = result.matrix[row][column] / div;
    }

    this->matrix[row][row] = 1.f;
  }

  for (int row = 0; row < 4; ++row) {
    for (int column = row + 1; column < 4; ++column) {
      float k = this->matrix[row][column];

      for (int j = 0; j < 4; ++j) {
        this->matrix[row][j] -= k * this->matrix[column][j];
        result.matrix[row][j] -= k * result.matrix[column][j];
      }

      this->matrix[row][column] = 0.f;
    }
  }

  return result;
}
