#pragma once

#include "../entities/GameObject.h"
#include "core/AssetStore.h"
#include "core/Window.h"
#include "core/IStrategy.h"

class MinimalLoopStrategy : public Core::IStrategy {
 public:
  GameObject ball;
  GameObject paddle;

  void Init(Core::Window& window, Core::Renderer& renderer) override {
    Core::AssetStore::Instance().AddFont("pico8", "assets/fonts/pico8.ttf", 24);

    ball = GameObject(15, 15, 20, 20, 300, 300);

    paddle = GameObject(
        100,
        20,
        (window.GetWidth() / 2) - (100 / 2),
        window.GetHeight() - 40,
        0,
        0
    );
  }

  void HandleEvent(SDL_Event& event) override {
    switch (event.type) {
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_LEFT) paddle.velX = -400;
        if (event.key.keysym.sym == SDLK_RIGHT) paddle.velX = +400;
        break;
      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_LEFT) paddle.velX = 0;
        if (event.key.keysym.sym == SDLK_RIGHT) paddle.velX = 0;
        break;
    }
  }

  void OnUpdate(
      Core::Window& window, Core::Renderer& renderer, double deltaTime
  ) override {
    // update ball and paddle position
    ball.x += ball.velX * deltaTime;
    ball.y += ball.velY * deltaTime;
    paddle.x += paddle.velX * deltaTime;
    paddle.y += paddle.velY * deltaTime;

    // If ball is lower than the left wall, force the position to be zero before
    // flipping the velocity
    if (ball.x <= 0) {
      ball.x = 0;
      ball.velX = -ball.velX;
    }

    // If ball is lower than the left wall, force the position to be zero before
    // flipping the velocity
    if (ball.y <= 0) {
      ball.y = 0;
      ball.velY = -ball.velY;
    }

    // If ball x plus its width is greater than window width, force position to
    // be at the right of the screen
    if (ball.x + ball.width > window.GetWidth()) {
      ball.x = window.GetWidth() - ball.width;
      ball.velX = -ball.velX;
    }

    // If ball y plus its height is greater than window height, force position
    // to be at the bottom of the screen
    if (ball.x + ball.width > window.GetWidth()) {
      ball.x = window.GetWidth() - ball.width;
      ball.velX = -ball.velX;
    }

    // Check for ball collision with the paddle and force the ball position to
    // be exactly on top of the paddle
    if (ball.y + ball.height >= paddle.y && ball.x + ball.width >= paddle.x &&
        ball.x <= paddle.x + paddle.width) {
      ball.y = paddle.y - ball.height;
      ball.velY = -ball.velY;
    }

    // Prevent paddle from moving outside the boundaries of the window
    if (paddle.x <= 0) paddle.x = 0;
    if (paddle.x >= window.GetWidth() - paddle.width)
      paddle.x = window.GetWidth() - paddle.width;

    // Check for game over
    if (ball.y + ball.height > window.GetHeight()) {
      ball.x = window.GetWidth() / 2;
      ball.y = 0;
    }
  }

  void OnRender(Core::Window& window, Core::Renderer& renderer) override {
    ball.Render(renderer);
    paddle.Render(renderer);
  }
};
