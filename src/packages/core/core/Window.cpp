#include "./Window.h"
#include <SDL_ttf.h>

namespace Core {

Window::Window(int width, int height, const char *title) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    throw std::runtime_error("Failed to initialize SDL");
  }
  if (TTF_Init()) {
    throw std::runtime_error("Failed to initialize SDL_ttf");
  }

  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);

  window = std::shared_ptr<SDL_Window>(
      SDL_CreateWindow(
          title,
          SDL_WINDOWPOS_CENTERED,
          SDL_WINDOWPOS_CENTERED,
          width,
          height,
          SDL_WINDOW_RESIZABLE
      ),
      SDL_DestroyWindow
  );

  if (window == nullptr) {
    throw std::runtime_error("Failed to create SDL window");
  }
}

std::shared_ptr<SDL_Window> Window::Get() { return window; }

int Window::GetWidth() {
  int windowWidth;
  SDL_GetWindowSize(window.get(), &windowWidth, nullptr); // Only get the width
  return windowWidth;
}

int Window::GetHeight() {
  int windowHeight;
  SDL_GetWindowSize(window.get(), nullptr, &windowHeight); // Only get the height
  return windowHeight;
}

}  // namespace Core
