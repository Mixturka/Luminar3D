#include "LuminarController.hpp"

LuminarController::LuminarController(LuminarView& view, Camera& camera, Mesh& mesh) :
  view_(view), camera_(camera), mesh_(mesh) {}

void LuminarController::HandleInput(SDL_Event& event) {
  const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);
  while (SDL_PollEvent(&event) != 0) {
    if (event.type == SDL_QUIT) {
      quit = true;} // UNCOMMENT TO ROTATE
    // } else if (event.type == SDL_MOUSEBUTTONDOWN) {
    //   OnMousePressed(event, keyboard_state_array);
    //   } else if (event.type == SDL_MOUSEMOTION) {
    //   Vec2<int> cursor_pos;
    //   SDL_GetMouseState(&cursor_pos.x, &cursor_pos.y);
    //   OnMouseMoved(cursor_pos);
    // } else if (event.type == SDL_MOUSEWHEEL) {
    //   OnMouseWheel(event.wheel.y);
    // } else if (event.type == SDL_MOUSEBUTTONUP) {
    //   OnMouseUp();
    // }
  }
}

void LuminarController::OnMousePressed(SDL_Event& event, const Uint8* keyboard_state) {
  int mouse_x, mouse_y;
  SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
  camera_.mouse_pos_ = Vec2<int>(mouse_x, mouse_y);
  if (event.button.button == SDL_BUTTON_MIDDLE && keyboard_state[SDL_SCANCODE_LSHIFT]) {
    camera_.move_type_ = Camera::CameraMoveType::TRACK;
  } else if (event.button.button == SDL_BUTTON_MIDDLE && keyboard_state[SDL_SCANCODE_LGUI]) {
    camera_.move_type_ = Camera::CameraMoveType::DOLLY;
  } else if (event.button.button == SDL_BUTTON_MIDDLE) {
    camera_.move_type_ = Camera::CameraMoveType::TUMBLE;
  } else {
    camera_.move_type_ = Camera::CameraMoveType::NONE;
  }
}

void LuminarController::OnMouseUp() {
  camera_.move_type_ = Camera::CameraMoveType::NONE;
}

void LuminarController::OnMouseMoved(Vec2<int>& point) {
  //luminar_.cam_.mouse_pos_ = point;
  static Vec2<int> last_mouse_pos = point;
  Vec2<int> delta(point.x - last_mouse_pos.x, point.y - last_mouse_pos.y);
  //luminar_.cam_.mouse_pos_ = point;
  last_mouse_pos = point;

  if (camera_.move_type_ == Camera::CameraMoveType::NONE) return;

  switch (camera_.move_type_) {
    case Camera::CameraMoveType::TUMBLE: {
      camera_.rotation_params_.x -= delta.x * camera_.kRotateAmplitude;
      camera_.rotation_params_.y -= delta.y * camera_.kRotateAmplitude;
      UpdateCameraRotation();
    }
      break;
    case Camera::CameraMoveType::DOLLY: {
      OnMouseWheel(delta.x + delta.y);
    }
      break;
    case Camera::CameraMoveType::TRACK: {
      Vec3<float> target_offset;
      target_offset = Vec3<float>(-camera_.kPanAmplitude * delta.x, 
                                  camera_.kPanAmplitude * delta.y,
                                  0) * camera_.camera_rotation_;

      camera_.look_at_ = camera_.look_at_ + target_offset;
    }
      break;

    case Camera::CameraMoveType::NONE: break;
  }

  camera_.SetCameraMatrices();
}

void LuminarController::UpdateCameraRotation() {
  Quat<float> q1, q2;
  q1.SetAxisAngle(Vec3<float>(1, 0, 0), camera_.rotation_params_.y);
  q2.SetAxisAngle(Vec3<float>(0, 1, 0), camera_.rotation_params_.x);

  Quat<float> q3 = q2 * q1;

  camera_.camera_rotation_ = q3.ToMat4();
}

void LuminarController::OnMouseWheel(int scroll_amount) {
  camera_.distance_to_target -= scroll_amount * camera_.kScrollAmplitude;
  camera_.SetCameraMatrices();
} 