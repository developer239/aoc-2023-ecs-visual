#include <gtest/gtest.h>
#include "../PongE2ETest.h"

TEST_F(PongE2ETest, PaddleMovesLeftOnLeftKeyPress) {
  SDL_Event keyDownEvent;
  keyDownEvent.type = SDL_KEYDOWN;
  keyDownEvent.key.keysym.sym = SDLK_LEFT;

  strategy.HandleEvent(keyDownEvent);

  RunGameLoopForFrames(1);

  EXPECT_EQ(strategy.paddle.velX, -400) << "Paddle should move left on left key press";
}

TEST_F(PongE2ETest, PaddleStopsMovingLeftOnLeftKeyUp) {
  SDL_Event keyUpEvent;
  keyUpEvent.type = SDL_KEYUP;
  keyUpEvent.key.keysym.sym = SDLK_LEFT;

  strategy.HandleEvent(keyUpEvent);

  RunGameLoopForFrames(1);

  EXPECT_EQ(strategy.paddle.velX, 0) << "Paddle should stop moving on left key up";
}

TEST_F(PongE2ETest, PaddleMovesRightOnRightKeyPress) {
  SDL_Event keyDownEvent;
  keyDownEvent.type = SDL_KEYDOWN;
  keyDownEvent.key.keysym.sym = SDLK_RIGHT;

  strategy.HandleEvent(keyDownEvent);

  RunGameLoopForFrames(1);

  EXPECT_EQ(strategy.paddle.velX, 400) << "Paddle should move right on right key press";
}

TEST_F(PongE2ETest, PaddleStopsMovingRightOnRightKeyUp) {
  SDL_Event keyUpEvent;
  keyUpEvent.type = SDL_KEYUP;
  keyUpEvent.key.keysym.sym = SDLK_RIGHT;

  strategy.HandleEvent(keyUpEvent);

  RunGameLoopForFrames(1);

  EXPECT_EQ(strategy.paddle.velX, 0) << "Paddle should stop moving on right key up";
}

TEST_F(PongE2ETest, PaddleMovementLimits) {
  strategy.paddle.x = 0;
  strategy.paddle.velX = -10;
  RunGameLoopForFrames(1);
  EXPECT_EQ(strategy.paddle.x, 0) << "Paddle should not move left beyond screen";

  strategy.paddle.x = PongE2ETest::WINDOW_WIDTH - strategy.paddle.width;
  strategy.paddle.velX = 10;
  RunGameLoopForFrames(1);
  EXPECT_EQ(strategy.paddle.x, PongE2ETest::WINDOW_WIDTH - strategy.paddle.width) << "Paddle should not move right beyond screen";
}
