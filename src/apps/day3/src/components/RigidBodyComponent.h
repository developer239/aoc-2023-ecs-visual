#pragma once

#include "../common/Vec2.h"

struct RigidBodyComponent {
  int width;
  int height;
  Vec2 position;
  bool filled;
  SDL_Color color;

  RigidBodyComponent(
      int width = 0, int height = 0, Vec2 position = Vec2(0.0, 0.0), bool filled = true, SDL_Color color = {255, 255, 255, 255}
  )
      : width(width), height(height), position(position), filled(filled), color(color) {}
};
