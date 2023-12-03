#pragma once

#include "../common/Vec2.h"

struct KeyboardControlledComponent {
  Vec2 upVelocity;
  Vec2 rightVelocity;
  Vec2 downVelocity;
  Vec2 leftVelocity;

  explicit KeyboardControlledComponent(
      Vec2 upVelocity = Vec2(0), Vec2 rightVelocity = Vec2(0),
      Vec2 downVelocity = Vec2(0), Vec2 leftVelocity = Vec2(0)
  )
      : upVelocity(upVelocity),
        rightVelocity(rightVelocity),
        downVelocity(downVelocity),
        leftVelocity(leftVelocity) {}
};
