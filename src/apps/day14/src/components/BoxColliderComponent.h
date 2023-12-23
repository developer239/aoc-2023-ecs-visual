#pragma once

#include <SDL.h>
#include "../common/Vec2.h"

struct BoxColliderComponent {
  int width;
  int height;
  Vec2 offset;
  SDL_Color color;

  explicit BoxColliderComponent(
      int width = 0, int height = 0, Vec2 offset = Vec2(0),
      SDL_Color color = {255, 0, 0, 255}
  )
      : width(width), height(height), offset(offset), color(color) {}
};
