#include "LuminarView.hpp"

LuminarView::LuminarView(int width, int height) : screen_width_(width), screen_height_(height) {
  image_buf_.resize(screen_height_);
  for (int i = 0; i < screen_height_; ++i) {
    image_buf_[i].resize(screen_width_);
    for (int j = 0; j < screen_width_; ++j) {
      image_buf_[i][j] = {0, 0, 0};
    }
  }
  depth_buf_.resize(screen_height_);
  for (int i = 0; i < screen_height_; ++i) {
    depth_buf_[i].resize(screen_width_);
    for (int j = 0; j < screen_width_; ++j) {
      depth_buf_[i][j] = 1000;
    }
  }
  //std::fill(image_buf_.begin(), image_buf_.end(), std::vector<Vec3<uint>>(screen_width_, {0, 0, 0}));
  //depth_buf_.resize(screen_height_, std::vector<float>(screen_width_, 60.0f)); // Initialize with far clipping value
}

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

  raster_x = static_cast<int>((pos_ndc_x + 1) / 2 * image_width);
  raster_y = static_cast<int>((1 - pos_ndc_y) / 2 * image_height);
}

void LuminarView::ClearBuffer() {
  for (auto& row : image_buf_) {
    row = std::vector<Vec3<uint>>(1920, Vec3<uint>{0, 0, 0});
  }
  for (auto& row : depth_buf_) {
    std::fill(row.begin(), row.end(), 60.0f);
  }
}

void LuminarView::Draw(const Camera& cam, Mesh& mesh) {
  Matrix4 worldToCamera = camera_to_world.Inverse();

  int width = screen_width_;
  int height = screen_height_;

  ClearBuffer();

  for (const auto& face : mesh.faces_) {
    int ind_1 = face.x;
    int ind_2 = face.y;
    int ind_3 = face.z;

    int raster_x1, raster_y1, raster_x2, raster_y2, raster_x3, raster_y3;

    ComputePixelCoords(mesh.vertices_[ind_1 - 1], worldToCamera, cam.bottom_, cam.left_, cam.top_, cam.right_, cam.near_clipping_, width, height, raster_x1, raster_y1);
    ComputePixelCoords(mesh.vertices_[ind_2 - 1], worldToCamera, cam.bottom_, cam.left_, cam.top_, cam.right_, cam.near_clipping_, width, height, raster_x2, raster_y2);
    ComputePixelCoords(mesh.vertices_[ind_3 - 1], worldToCamera, cam.bottom_, cam.left_, cam.top_, cam.right_, cam.near_clipping_, width, height, raster_x3, raster_y3);

    Vec3<float> v0_raster{static_cast<float>(raster_x1), static_cast<float>(raster_y1), mesh.vertices_[ind_1 - 1].z};
    Vec3<float> v1_raster{static_cast<float>(raster_x2), static_cast<float>(raster_y2), mesh.vertices_[ind_2 - 1].z};
    Vec3<float> v2_raster{static_cast<float>(raster_x3), static_cast<float>(raster_y3), mesh.vertices_[ind_3 - 1].z};

    float area = EdgeFunction(v0_raster, v1_raster, v2_raster);

    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        Vec3<float> p{static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f, 0};

        float w0 = EdgeFunction(v1_raster, v2_raster, p);
        float w1 = EdgeFunction(v2_raster, v0_raster, p);
        float w2 = EdgeFunction(v0_raster, v1_raster, p);

        if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
          w0 /= area;
          w1 /= area;
          w2 /= area;

          float one_over_z = v0_raster.z * w0 + v1_raster.z * w1 + v2_raster.z * w2;
          float depth = 1.0f / one_over_z;

          if (depth < depth_buf_[y][x]) {
            depth_buf_[y][x] = depth;

            Vec3<float> c0{0, 0, 1};
            Vec3<float> c1{0, 1, 0};
            Vec3<float> c2{1, 0, 0};

            float r = w0 * c0.x + w1 * c1.x + w2 * c2.x;
            float g = w0 * c0.y + w1 * c1.y + w2 * c2.y;
            float b = w0 * c0.z + w1 * c1.z + w2 * c2.z;

            r *= one_over_z;
            g *= one_over_z;
            b *= one_over_z;

            image_buf_[y][x] = Vec3<uint>{static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255)};
          }
        }
      }
    }
  }

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer_);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      SDL_SetRenderDrawColor(renderer_, image_buf_[y][x].x, image_buf_[y][x].y, image_buf_[y][x].z, 255);
      SDL_RenderDrawPoint(renderer_, x, y);
    }
  }

  SDL_RenderPresent(renderer_);
}

float LuminarView::EdgeFunction(const Vec3<float>& v0, const Vec3<float>& v1, const Vec3<float>& point) {
  return (point.x - v0.x) * (v1.y - v0.y) - (point.y - v0.y) * (v1.x - v0.x);
}

void LuminarView::Rasterize() {
  // Implementation for future rasterization logic can be added here.
}