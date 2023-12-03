#pragma once

#include "../common/Vec2.h"

struct RigidBodyComponent {
  int width;
  int height;
  Vec2 velocity;
  Vec2 position;
  bool filled;  // Indicates whether the body is filled or just a border

  RigidBodyComponent(
      int width = 0, int height = 0, Vec2 position = Vec2(0.0, 0.0),
      Vec2 velocity = Vec2(0.0, 0.0), bool filled = true
  )
      : width(width), height(height), velocity(velocity), position(position), filled(filled) {}
};
