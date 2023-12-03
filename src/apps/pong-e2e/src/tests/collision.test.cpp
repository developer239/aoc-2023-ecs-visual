#include <gtest/gtest.h>
#include "../PongE2ETest.h"

TEST_F(PongE2ETest, BallBouncesOffPaddle) {
  strategy.ball.x =
      (strategy.paddle.x + strategy.paddle.width / 2) - strategy.ball.width / 2;
  strategy.ball.y = strategy.paddle.y - strategy.ball.height - 10;

  strategy.ball.velY = 10;
  strategy.ball.velX = 0;

  RunGameLoopForFrames(1);
  EXPECT_EQ(strategy.ball.velY, 10) << "Just before ball hits the paddle";

  RunGameLoopForFrames(1);
  EXPECT_EQ(strategy.ball.velY, -10) << "Ball hits the paddle";
}

TEST_F(PongE2ETest, BallBouncesOffLeftWall) {
  strategy.ball.x = 10;
  strategy.ball.y = PongE2ETest::WINDOW_HEIGHT / 2;

  strategy.ball.velX = -10;
  strategy.ball.velY = 0;

  RunGameLoopForFrames(1);
  EXPECT_EQ(strategy.ball.velX, -10) << "Just before ball hits the left wall";

  RunGameLoopForFrames(1);
  EXPECT_EQ(strategy.ball.velX, 10)
      << "Ball should bounce off the left wall and move right";
}

TEST_F(PongE2ETest, BallBouncesOffRighttWall) {
  strategy.ball.x =
      PongE2ETest::WINDOW_WIDTH - strategy.ball.width - 10;
  strategy.ball.y = PongE2ETest::WINDOW_HEIGHT / 2;

  strategy.ball.velX = 10;
  strategy.ball.velY = 0;

  RunGameLoopForFrames(1);
  EXPECT_EQ(strategy.ball.velX, 10) << "Just before ball hits the left wall";

  RunGameLoopForFrames(1);
  EXPECT_EQ(strategy.ball.velX, -10)
      << "Ball should bounce off the left wall and move right";
}

TEST_F(PongE2ETest, GameOverResetsBallPosition) {
  strategy.ball.x =
      PongE2ETest::WINDOW_WIDTH - strategy.ball.width * 3;
  strategy.ball.y =
      PongE2ETest::WINDOW_HEIGHT - strategy.ball.height - 10;

  strategy.ball.velY = 10;
  strategy.ball.velX = 0;

  RunGameLoopForFrames(2);

  EXPECT_EQ(strategy.ball.x, PongE2ETest::WINDOW_WIDTH / 2)
      << "Ball should be reset to the top center of the window";
  EXPECT_EQ(strategy.ball.y, 0)
      << "Ball should be reset to the top of the window";
}

TEST_F(PongE2ETest, BallCollidesWithPaddleCorner) {
  strategy.ball.x = strategy.paddle.x - strategy.ball.width - 10;
  strategy.ball.y = strategy.paddle.y - strategy.ball.height - 10;

  strategy.ball.velX = 10;
  strategy.ball.velY = 10;

  RunGameLoopForFrames(2);

  EXPECT_EQ(strategy.ball.velX, 10) << "Ball should bounce to the right";
  EXPECT_EQ(strategy.ball.velY, -10) << "Ball should continue moving upward";
}
