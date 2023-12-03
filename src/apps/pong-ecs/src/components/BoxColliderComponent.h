#pragma once

#include "../common/Vec2.h"

struct BoxColliderComponent {
  int width;
  int height;
  Vec2 offset;

  explicit BoxColliderComponent(
      int width = 0, int height = 0, Vec2 offset = Vec2(0)
  )
      : width(width), height(height), offset(offset) {}
};
