#include "PongE2ETest.h"

const int PongE2ETest::WINDOW_WIDTH = 500;
const int PongE2ETest::WINDOW_HEIGHT = 500;

MinimalLoopStrategy PongE2ETest::strategy;
std::unique_ptr<Core::Loop> PongE2ETest::gameLoop;

void PongE2ETest::SetUpTestCase() {
  gameLoop = std::make_unique<Core::Loop>(
      std::vector<Core::IStrategy *>{},
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      "Pong E2E Test"
  );

  // Init SDL Window
  RunGameLoopForFrames(1);

  gameLoop->AddStrategy(&strategy);
}

void PongE2ETest::RunGameLoopForFrames(int frames) {
  for (int i = 0; i <= frames; i++) {
    gameLoop->RunOneFrame();
    SDL_Delay(400);
  }
}
