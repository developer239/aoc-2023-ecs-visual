#pragma once

#include "../common/Vec2.h"

// TODO: this should be split int multiple components
struct RigidBodyComponent {
  int width;
  int height;
  Vec2 position;
  bool filled;
  SDL_Color color;
  bool isFixed = false;
  Vec2 velocity;
  Vec2 initialPosition;

  RigidBodyComponent(
      int width = 0, int height = 0, Vec2 position = Vec2(0.0, 0.0), bool filled = true, SDL_Color color = {255, 255, 255, 255}, bool isFixed = false, Vec2 velocity = Vec2(0.0, 0.0)
  )
      : width(width), height(height), position(position), filled(filled), color(color), isFixed(isFixed), velocity(velocity), initialPosition(position) {}
};
