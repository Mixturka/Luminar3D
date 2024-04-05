#include "Luminar.h"

void Luminar::Init(Camera cam, Mesh& mesh) {
  cam_ = cam;
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  screen_height = 720;
  screen_width = 1280;

  SDL_Window* window = nullptr;
  SDL_Surface* screen_surface = nullptr;
  SDL_Renderer* renderer = nullptr;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL couldn't be initialized! SDL error: \n" << SDL_GetError();
  } else {
    window = SDL_CreateWindow("Luminar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              screen_width, screen_height, SDL_WINDOW_SHOWN);
 
    if (window == nullptr) {
      std::cout << "Window couldn't be created! SDL error: \n" << SDL_GetError();
    } else {
      renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
      screen_surface = SDL_GetWindowSurface(window);
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

      SDL_Event event;
      bool quit = false;

      while(quit == false) { 
        while(SDL_PollEvent(&event)) { 
          if (event.type == SDL_QUIT) quit = true; 
        } 
        
        
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          Matrix4 cameraToWorld(-0.95424, 0, 0.299041, 0, 0.0861242, 0.95763, 0.274823, 0, -0.28637, 0.288002, -0.913809, 0, -3.734612, 7.610426, -14.152769, 1);
  Matrix4 worldToCamera = cameraToWorld.Inverse();
  int width = 1280;
  int height = 720;
  for (const auto& face : mesh.faces_) {
    int ind_1 = face.x;
    int ind_2 = face.y;
    int ind_3 = face.z;

    int raster_x1, raster_y1;
    int raster_x2, raster_y2;
    int raster_x3, raster_y3;

    ComputePixelCoords(mesh.vertices_[ind_1 - 1], worldToCamera, cam_.bottom_, cam_.left_, cam_.top_, cam_.right_, cam_.near_clipping_, width, height, raster_x1, raster_y1);

    ComputePixelCoords(mesh.vertices_[ind_2 - 1], worldToCamera, cam_.bottom_, cam_.left_, cam_.top_, cam_.right_, cam_.near_clipping_, width, height, raster_x2, raster_y2);

    ComputePixelCoords(mesh.vertices_[ind_3 - 1], worldToCamera, cam_.bottom_, cam_.left_, cam_.top_, cam_.right_, cam_.near_clipping_, width, height, raster_x3, raster_y3);
    
      SDL_RenderDrawLine(renderer, raster_x1, raster_y1, raster_x2, raster_y2);
      SDL_RenderDrawLine(renderer, raster_x2, raster_y2, raster_x3, raster_y3);
      SDL_RenderDrawLine(renderer, raster_x1, raster_y1, raster_x3, raster_y3);
      }

      //SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
     // Draw(mesh);

     SDL_RenderPresent(renderer);
    }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
}

void Luminar::ComputePixelCoords(const Vec3<float>& p_world, const Matrix4& world_to_camera,
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

  std::cout << "DEBUG " << raster_x << ' ' << raster_y << '\n';
}

void Luminar::Draw(Mesh mesh) {
  Matrix4 cameraToWorld(-0.95424, 0, 0.299041, 0, 0.0861242, 0.95763, 0.274823, 0, -0.28637, 0.288002, -0.913809, 0, -3.734612, 7.610426, -14.152769, 1);

  Matrix4 worldToCamera = cameraToWorld.Inverse();
  int width = 1280;
  int height = 720;
  for (const auto& face : mesh.faces_) {
    int ind_1 = face.x;
    int ind_2 = face.y;
    int ind_3 = face.z;

    int raster_x1, raster_y1;
    int raster_x2, raster_y2;
    int raster_x3, raster_y3;

    ComputePixelCoords(mesh.vertices_[ind_1 - 1], worldToCamera, cam_.bottom_, cam_.left_, cam_.top_, cam_.right_, cam_.near_clipping_, width, height, raster_x1, raster_y1);

    ComputePixelCoords(mesh.vertices_[ind_2 - 1], worldToCamera, cam_.bottom_, cam_.left_, cam_.top_, cam_.right_, cam_.near_clipping_, width, height, raster_x2, raster_y2);

    ComputePixelCoords(mesh.vertices_[ind_3 - 1], worldToCamera, cam_.bottom_, cam_.left_, cam_.top_, cam_.right_, cam_.near_clipping_, width, height, raster_x3, raster_y3);


}

  for (auto& vert : mesh.vertices_) {
    int raster_x, raster_y;

    ComputePixelCoords(vert, worldToCamera, cam_.bottom_, cam_.left_, cam_.top_, cam_.right_, cam_.near_clipping_, width, height, raster_x, raster_y);

    std::cout << "RASTER: " << raster_x << ' ' << raster_y << '\n';
  }
}