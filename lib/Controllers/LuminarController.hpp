#pragma once
#include "../Views/LuminarView.hpp"
#include "../Math/Vec2.h"
#include "../Math/Quat.h"

#include "/Users/deniv/Documents/Luminar3D/SDL2/include/SDL.h"

class LuminarController {
 public:
  LuminarController(LuminarView& view, Camera& camera, Mesh& mesh);
  
  void HandleInput(SDL_Event& event);

  void OnMousePressed(SDL_Event& event, const Uint8* keyboard_state);

  void OnMouseUp();

  void OnMouseMoved(Vec2<int>& point);

  void OnMouseWheel(int scroll_amount);

  void UpdateCameraRotation();

 private:
  LuminarView& view_;
  Camera& camera_;
  Mesh& mesh_;
  
 public:
  bool quit = false;
};