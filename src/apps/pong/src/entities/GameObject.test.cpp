#include "GameObject.h"
#include <gtest/gtest.h>

class GameObjectTest : public testing::Test {
 protected:
  GameObject gameObject;
};

TEST_F(GameObjectTest, ConstructorInitialization) {
  GameObject gameObject(10, 20, 30, 40, 50, 60);

  EXPECT_EQ(gameObject.width, 10);
  EXPECT_EQ(gameObject.height, 20);
  EXPECT_EQ(gameObject.x, 30);
  EXPECT_EQ(gameObject.y, 40);
  EXPECT_EQ(gameObject.velX, 50);
  EXPECT_EQ(gameObject.velY, 60);
}
