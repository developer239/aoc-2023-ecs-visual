#pragma once

#include <SDL.h>
#include <gtest/gtest.h>
#include "../../pong/src/strategies/MinimalLoopStrategy.h"
#include "core/Loop.h"

class PongE2ETest : public ::testing::Test {
 public:
  static const int WINDOW_WIDTH;
  static const int WINDOW_HEIGHT;

 protected:
  static MinimalLoopStrategy strategy;
  static std::unique_ptr<Core::Loop> gameLoop;

  static void SetUpTestCase();

  void SetUp() override {
    strategy.ball.velX = 0;
    strategy.ball.velY = 0;
  }

  static void RunGameLoopForFrames(int frames);
};
