#pragma once

#include "../common/Vec2.h"

struct CameraComponent {
  Vec2 position;
  int width;
  int height;

  CameraComponent(Vec2 position = Vec2(0, 0), int width = 0, int height = 0)
      : position(position), width(width), height(height){};
};
