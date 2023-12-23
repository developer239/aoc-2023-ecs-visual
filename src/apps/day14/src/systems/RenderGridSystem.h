#pragma once

#include "core/Renderer.h"
#include "core/Window.h"
#include "ecs/System.h"

class RenderGridSystem : public ECS::System {
 public:
  RenderGridSystem(float scaleX, float scaleY)
      : scaleX(scaleX), scaleY(scaleY) {}

  void Render(Core::Renderer renderer, Core::Window window) {
    SDL_SetRenderDrawColor(
        renderer.Get().get(),
        200,
        200,
        200,
        SDL_ALPHA_OPAQUE
    );

    int numHorizontalLines = window.GetHeight() / scaleY;
    int numVerticalLines = window.GetWidth() / scaleX;

    for (int i = 0; i <= numHorizontalLines; ++i) {
      int y = i * scaleY;
      for (int x = 0; x < window.GetWidth();
           x += 10) {
        SDL_RenderDrawLine(
            renderer.Get().get(),
            x,
            y,
            x + 5,
            y
        );
      }
    }

    for (int i = 0; i <= numVerticalLines; ++i) {
      int x = i * scaleX;
      for (int y = 0; y < window.GetHeight();
           y += 10) {
        SDL_RenderDrawLine(
            renderer.Get().get(),
            x,
            y,
            x,
            y + 5
        );
      }
    }

    SDL_SetRenderDrawColor(
        renderer.Get().get(),
        0,
        0,
        0,
        SDL_ALPHA_OPAQUE
    );
  }

 private:
  float scaleX, scaleY;
};
