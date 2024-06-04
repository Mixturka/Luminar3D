#pragma once
#include <iostream>

#include "Matrix.h"

template <typename T>
struct Vec2 {
  T x;
  T y;

  Vec2(T x_, T y_);
  Vec2() = default;
  Vec2(const Vec2& other);
  Vec2(Vec2&& other);

  Vec2& operator=(Vec2& other);

  Vec2& operator=(Vec2&& other);
};

template <typename T>
Vec2<T>::Vec2(T x_, T y_) : x(x_), y(y_){}

template <typename T>
Vec2<T>& Vec2<T>::operator=(Vec2<T>& other) {
  this->x = other.x;
  this->y = other.y;

  return *this;
}

template <typename T>
Vec2<T>& Vec2<T>::operator=(Vec2<T>&& other) {
  if (this != &other) {
    x = std::move(other.x);
    y = std::move(other.y);
  }

  return *this;
}

template <typename T>
Vec2<T>::Vec2(const Vec2<T>& other) : x(other.x), y(other.y) {}

template <typename T>
Vec2<T>::Vec2(Vec2<T>&& other) : x(std::move(other.x)), y(std::move(other.y)) {}