#pragma once

#include <SDL.h>

#include <memory>

namespace Core {

class Window {
  std::shared_ptr<SDL_Window> window;

 public:
  Window(int width = 800, int height = 600, const char *title = "CPP Starter");

  std::shared_ptr<SDL_Window> Get();

  int GetWidth();

  int GetHeight();
};

}  // namespace Core
