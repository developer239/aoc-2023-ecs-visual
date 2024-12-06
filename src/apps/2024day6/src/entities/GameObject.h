#pragma once

#include <SDL.h>
#include "core/Renderer.h"

struct GameObject {
  double x;
  double y;
  double width;
  double height;
  double velX;
  double velY;
  std::vector<Uint8> color;

  explicit GameObject(
      float width = 0, float height = 0, float x = 0, float y = 0,
      float vel_x = 0, float vel_y = 0,
      std::vector<Uint8> color = {255, 255, 255}
  ) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->velX = vel_x;
    this->velY = vel_y;
    this->color = color;
  }

  void Render(Core::Renderer& renderer) const {
    SDL_Rect rect =
        {(int)this->x, (int)this->y, (int)this->width, (int)this->height};
    SDL_SetRenderDrawColor(
        renderer.Get().get(),
        color[0],
        color[1],
        color[2],
        255
    );
    SDL_RenderFillRect(renderer.Get().get(), &rect);
  }
};
