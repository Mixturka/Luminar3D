#include "LuminarView.hpp"

LuminarView::LuminarView(int width, int height) : screen_width_(width), screen_height_(height) {}

LuminarView::~LuminarView() {
  if (renderer_) {
    SDL_DestroyRenderer(renderer_);
  }
  if (window_) {
    SDL_DestroyWindow(window_);
  }

  SDL_Quit();
}

void LuminarView::Init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL couldn't be initialized! SDL error: \n" << SDL_GetError();
    exit(EXIT_FAILURE);
  }

  window_ = SDL_CreateWindow("Luminar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             screen_width_, screen_height_, SDL_WINDOW_SHOWN);

  if (window_ == nullptr) {
    std::cout << "Window couldn't be created! SDL error: \n" << SDL_GetError();
    exit(EXIT_FAILURE);
  }

  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED); 
  // SDL_RENDERER_SOFTWARE - another option

  if (renderer_ == nullptr) {
    std::cout << "Renderer couldn't be created! SDL error: \n" << SDL_GetError();
    exit(EXIT_FAILURE);
  }

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
}

void LuminarView::ComputePixelCoords(const Vec3<float>& p_world, const Matrix4& world_to_camera,
                                 const float& bottom, const float& left, const float& top,
                                 const float& right, const float& near, const int image_width,
                                 const int image_height, int& raster_x, int& raster_y) {
  Vec3<float> pos_camera = p_world * world_to_camera;
  
  float pos_screen_x = pos_camera.x / -pos_camera.z * near;
  float pos_screen_y = pos_camera.y / -pos_camera.z * near;

  float pos_ndc_x = (pos_screen_x - left) / (right - left) * 2 - 1;
  float pos_ndc_y = (pos_screen_y - bottom) / (top - bottom) * 2 - 1;

  raster_x = (int)((pos_ndc_x + 1) / 2 * image_width);
  raster_y = (int)((1 - pos_ndc_y) / 2 * image_height);
}

void LuminarView::Draw(const Camera& cam, Mesh& mesh) {
   Matrix4 worldToCamera = camera_to_world.Inverse();
  int width = 1280;
  int height = 720;

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer_);
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);

  for (const auto& face : mesh.faces_) {
    int ind_1 = face.x;
    int ind_2 = face.y;
    int ind_3 = face.z;

    int raster_x1, raster_y1;
    int raster_x2, raster_y2;
    int raster_x3, raster_y3;

    ComputePixelCoords(mesh.vertices_[ind_1 - 1], worldToCamera, cam.bottom_, cam.left_, cam.top_, cam.right_, cam.near_clipping_, width, height, raster_x1, raster_y1);

    ComputePixelCoords(mesh.vertices_[ind_2 - 1], worldToCamera, cam.bottom_, cam.left_, cam.top_, cam.right_, cam.near_clipping_, width, height, raster_x2, raster_y2);

    ComputePixelCoords(mesh.vertices_[ind_3 - 1], worldToCamera, cam.bottom_, cam.left_, cam.top_, cam.right_, cam.near_clipping_, width, height, raster_x3, raster_y3);
    
    SDL_RenderDrawLine(renderer_, raster_x1, raster_y1, raster_x2, raster_y2);
    SDL_RenderDrawLine(renderer_, raster_x2, raster_y2, raster_x3, raster_y3);
    SDL_RenderDrawLine(renderer_, raster_x1, raster_y1, raster_x3, raster_y3);
  }
  SDL_RenderPresent(renderer_);
}

float LuminarView::EdgeFunction(const Vec2<float>& v0, const Vec2<float>& v1, 
                                const Vec3<float>& point) {
  return (point.x - v0.x) * (v1.y - v0.y) - (point.y - v0.y) * (v1.x - v0.x);
}