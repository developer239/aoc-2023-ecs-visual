#pragma once

#include <SDL.h>

#include <memory>
#include <vector>

#include "./IStrategy.h"
#include "./Renderer.h"
#include "./Window.h"

namespace Core {

class Loop {
  Window window;
  Renderer renderer;

  std::vector<IStrategy*> strategies;

  bool shouldQuit = false;

  SDL_Event event{};

 public:
  double msPreviousFrame;
  double deltaTime = 0;
  double fpsLimit = 60;

  explicit Loop(std::vector<IStrategy*> strategies, int windowWidth = 800, int windowHeight = 600, const char *title = "CPP Starter");

  void AddStrategy(IStrategy* strategy);

  void UpdateDeltaTime();

  void UpdateMsPreviousFrame();

  ~Loop();

  void Run();

  void RunOneFrame();
};

}  // namespace Core
