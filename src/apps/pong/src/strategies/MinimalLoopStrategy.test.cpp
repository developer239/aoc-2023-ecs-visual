#include "MinimalLoopStrategy.h"
#include <gtest/gtest.h>

class MinimalLoopStrategyTest : public testing::Test {
 protected:
  MinimalLoopStrategy strategy;
  Core::Window testWindow;
  Core::Renderer testRenderer;
  std::shared_ptr<SDL_Window> sdlWindow;

  MinimalLoopStrategyTest()
      : sdlWindow(
            SDL_CreateWindow(
                "Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,
                480, SDL_WINDOW_HIDDEN
            ),
            SDL_DestroyWindow
        ),
        testRenderer(sdlWindow),
        strategy() {}

  void SetUp() override { strategy.Init(testWindow, testRenderer); }
};

TEST_F(MinimalLoopStrategyTest, Initialization) {
  EXPECT_EQ(strategy.ball.width, 15);
  EXPECT_EQ(strategy.ball.height, 15);
  EXPECT_EQ(strategy.ball.x, 20);
  EXPECT_EQ(strategy.ball.y, 20);
  EXPECT_EQ(strategy.ball.velX, 300);
  EXPECT_EQ(strategy.ball.velY, 300);
}

TEST_F(MinimalLoopStrategyTest, HandleEventAndUpdate) {
  double deltaTime = 0.1; // 100 milliseconds for example

  //
  // Simulate a key down event for moving left
  SDL_Event keyDownEvent;
  keyDownEvent.type = SDL_KEYDOWN;
  keyDownEvent.key.keysym.sym = SDLK_LEFT;
  strategy.HandleEvent(keyDownEvent);

  // Check if the paddle velocity is set correctly
  EXPECT_EQ(strategy.paddle.velX, -400);

  // Simulate a small time step for OnUpdate
  strategy.OnUpdate(testWindow, testRenderer, deltaTime);

  // Check if the paddle position is updated correctly
  double initialPaddleXLEFT = strategy.paddle.x;
  strategy.OnUpdate(testWindow, testRenderer, deltaTime);
  double expectedPositionLEFT = initialPaddleXLEFT + strategy.paddle.velX * deltaTime;

  EXPECT_EQ(strategy.paddle.x, expectedPositionLEFT);

  //
  // Simulate a key down event for moving left
  // Reset and test for right movement
  keyDownEvent.type = SDL_KEYDOWN;
  keyDownEvent.key.keysym.sym = SDLK_RIGHT;
  strategy.HandleEvent(keyDownEvent);

  // Check if the paddle velocity is set correctly
  EXPECT_EQ(strategy.paddle.velX, 400);

  // Simulate a small time step for OnUpdate
  strategy.OnUpdate(testWindow, testRenderer, deltaTime);

  // Check if the paddle position is updated correctly
  double initialPaddleXRIGHT = strategy.paddle.x;
  strategy.OnUpdate(testWindow, testRenderer, deltaTime);
  double expectedPositionRIGHT = initialPaddleXRIGHT + strategy.paddle.velX * deltaTime;

  EXPECT_EQ(strategy.paddle.x, expectedPositionRIGHT);

  // Simulate for ball movement

  // RenderColorBuffer again and check position
  strategy.OnUpdate(testWindow, testRenderer, deltaTime);

  // Check if the paddle position is updated correctly
  double initialBallPositionX = strategy.ball.x;
  double initialBallPositionY = strategy.ball.y;
  strategy.OnUpdate(testWindow, testRenderer, deltaTime);
  double expectedBallPositionX = initialBallPositionX + strategy.ball.velX * deltaTime;
  double expectedBallPositionY = initialBallPositionY + strategy.ball.velY * deltaTime;

  EXPECT_EQ(strategy.ball.x, expectedBallPositionX);
  EXPECT_EQ(strategy.ball.y, expectedBallPositionY);
}
