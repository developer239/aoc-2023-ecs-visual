#pragma once

#include <SDL.h>

#include <memory>

#include "./Renderer.h"
#include "./Window.h"

namespace Core {

class IStrategy {
 public:
  virtual void Init(Window& window, Renderer& renderer) = 0;
  virtual void HandleEvent(SDL_Event& event) = 0;
  // OnBeforeRender and OnAfterRender are here mostly because of things like
  // ImGui that needs to be compatible with ECS strategy and custom GUI systems
  virtual void OnBeforeRender(Window& window, Renderer& renderer) {}
  virtual void OnRender(Window& window, Renderer& renderer) = 0;
  virtual void OnAfterRender(Window& window, Renderer& renderer) {}
  virtual void OnUpdate(
      Window& window, Renderer& renderer, double deltaTime
  ) = 0;
};

}  // namespace Core
