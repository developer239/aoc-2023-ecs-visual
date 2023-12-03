#pragma once

class Vec2 {
 public:
  float x;
  float y;

  explicit Vec2(float xy) {
    this->x = xy;
    this->y = xy;
  }

  explicit Vec2(float x, float y) {
    this->x = x;
    this->y = y;
  }
};
