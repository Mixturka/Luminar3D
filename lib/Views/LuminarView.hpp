#pragma once

#include "/Users/deniv/Documents/Luminar3D/SDL2/include/SDL.h"

#include "../Camera.h"
#include "../Model.h"
#include "../Vec3.h"
#include "../Vec2.h"
#include "../Matrix.h"

class LuminarView {
 public:
  LuminarView(int width, int height);
  ~LuminarView();

  void Init();
  void Draw(const Camera& cam, Mesh& mesh);

  void ComputePixelCoords(const Vec3<float>& p_world, const Matrix4& world_to_camera,
                    const float& bottom, const float& left, const float& top,
                    const float& right, const float& near, const int image_width,
                    const int image_height, int& raster_x, int& raster_y);

  float EdgeFunction(const Vec2<float>& v0, const Vec2<float>& v1, const Vec3<float>& point);

  void Rasterize();

 private:
  int screen_width_;
  int screen_height_;
  SDL_Window* window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;
};