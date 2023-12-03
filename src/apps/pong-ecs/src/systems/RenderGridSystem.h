#pragma once

#include "core/Renderer.h"
#include "core/Window.h"
#include "ecs/System.h"

class RenderGridSystem : public ECS::System {
 public:
  RenderGridSystem(float scaleX, float scaleY)
      : scaleX(scaleX), scaleY(scaleY) {}

  void Render(Core::Renderer renderer, Core::Window window) {
    // Set the color for the grid lines
    SDL_SetRenderDrawColor(
        renderer.Get().get(),
        200,
        200,
        200,
        SDL_ALPHA_OPAQUE
    );  // Light grey color

    // Calculate the number of lines to draw based on the window size and scale
    int numHorizontalLines = window.GetHeight() / scaleY;
    int numVerticalLines = window.GetWidth() / scaleX;

    // Draw horizontal lines
    for (int i = 0; i <= numHorizontalLines; ++i) {
      int y = i * scaleY;
      for (int x = 0; x < window.GetWidth();
           x += 10) {  // Draw dotted line: 10 pixels line, 10 pixels space
        SDL_RenderDrawLine(
            renderer.Get().get(),
            x,
            y,
            x + 5,
            y
        );  // Draw 5 pixels
      }
    }

    // Draw vertical lines
    for (int i = 0; i <= numVerticalLines; ++i) {
      int x = i * scaleX;
      for (int y = 0; y < window.GetHeight();
           y += 10) {  // Draw dotted line: 10 pixels line, 10 pixels space
        SDL_RenderDrawLine(
            renderer.Get().get(),
            x,
            y,
            x,
            y + 5
        );  // Draw 5 pixels
      }
    }

    // Reset the color for subsequent renderings
    SDL_SetRenderDrawColor(
        renderer.Get().get(),
        0,
        0,
        0,
        SDL_ALPHA_OPAQUE
    );  // Back to black
  }

 private:
  float scaleX, scaleY;
};
