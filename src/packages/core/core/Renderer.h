#pragma once

#include <SDL.h>

#include <memory>

namespace Core {

class Renderer {
  std::shared_ptr<SDL_Renderer> renderer;

 public:
  explicit Renderer(const std::shared_ptr<SDL_Window>& window);

  void Render();

  std::shared_ptr<SDL_Renderer> Get() { return renderer; }
};

}  // namespace Core