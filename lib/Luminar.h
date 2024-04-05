#pragma once
#include <cmath>
#include <iostream>

#include "Camera.h"
#include "Matrix.h"
#include "Model.h"
#include "Vec3.h"

#include "../SDL2/include/SDL.h"

static int screen_width;
static int screen_height;

class Luminar {
  public:
    void Init(Camera cam, Mesh& mesh);

    void Draw(Mesh mesh);

static void ComputePixelCoords(const Vec3<float>& p_world, const Matrix4& world_to_camera,
                        const float& bottom, const float& left, const float& top,
                        const float& right, const float& near, const int image_width,
                        const int image_height, int& raster_x, int& raster_y);


    void DrawCircle(SDL_Renderer* renderer, int x, int y, int radius);
  private:
  Camera cam_;
};