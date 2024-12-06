#pragma once
#include <fstream>
#include <iostream>

#include "../entities/GameObject.h"
#include "core/AssetStore.h"
#include "core/IStrategy.h"
#include "core/Window.h"

std::vector<std::string> ParseInput(const std::string& filePath) {
  std::vector<std::string> inputData;
  std::ifstream inFile(filePath);
  std::string line;

  while (std::getline(inFile, line)) {
    if (!line.empty()) {
      inputData.push_back(line);
    }
  }
  return inputData;
}

bool areOverlapping(GameObject& guard, GameObject& b) {
  return guard.x < b.x + b.width && guard.x + guard.width > b.x &&
         guard.y < b.y + b.height && guard.y + guard.height > b.y;
}

class MinimalLoopStrategy : public Core::IStrategy {
 public:
  std::vector<GameObject> obstructions;
  GameObject guard;
  std::vector<GameObject> path;
  double pathTotalVelocity = 0;

  int boundaryHeight;
  int boundaryWidth;

  int scale = 40;
  int defaultVelocity = 100;

  double totalTime = 0;
  double lastPositionChange = 0;

  void Init(Core::Window& window, Core::Renderer& renderer) override {
    Core::AssetStore::Instance().AddFont("pico8", "assets/fonts/pico8.ttf", 24);

    auto size = 1 * scale;

    const std::string filePath = "assets/input-example-1.txt";
    auto input = ParseInput(filePath);

    boundaryWidth = input[0].size() * size;
    boundaryHeight = input.size() * size;

    for (int y = 0; y < input.size(); y++) {
      auto line = input[y];
      for (int x = 0; x < line.size(); x++) {
        if (line[x] == '#') {
          float vel_x = 0;
          float vel_y = 0;
          float width = size;
          float height = size;

          obstructions.push_back(
              GameObject(width, height, x * size, y * size, vel_x, vel_y)
          );
        }

        if (line[x] == '^') {
          float vel_x = 0;
          float vel_y = -defaultVelocity;

          auto lolCollisionFix = 0;
          float width = size - lolCollisionFix;
          float height = size - lolCollisionFix;

          guard = GameObject(
              width,
              height,
              x * size,
              y * size,
              vel_x,
              vel_y,
              {255, 0, 0}
          );
        }
      }
    }
  }

  void HandleEvent(SDL_Event& event) override {
    switch (event.type) {
      case SDL_KEYDOWN:
        break;
      case SDL_KEYUP:
        break;
    }
  }

  void OnUpdate(
      Core::Window& window, Core::Renderer& renderer, double deltaTime
  ) override {
    totalTime += deltaTime;

    if (totalTime < 1) {
      return;
    }

    guard.x += guard.velX * deltaTime;
    guard.y += guard.velY * deltaTime;

    std::cout << totalTime << std::endl;

    for (auto& obstruction : obstructions) {
      GameObject guardTemp = guard;
      guardTemp.x += guard.velX * deltaTime;
      guardTemp.y += guard.velY * deltaTime;

      if (areOverlapping(guardTemp, obstruction) &&
          totalTime - lastPositionChange > 0.4) {
        auto pastVelY = guard.velY;
        auto pastVelX = guard.velX;

        guard.velY = 0;
        guard.velX = 0;

        if (pastVelY < 0) {
          std::cout << "a" << std::endl;
          guard.velY = 0;
          guard.velX = defaultVelocity;
        }

        if (pastVelX > 0) {
          std::cout << "b" << std::endl;

          guard.velX = 0;
          guard.velY = defaultVelocity;
        }

        if (pastVelY > 0) {
          std::cout << "c" << std::endl;

          guard.velY = 0;
          guard.velX = -defaultVelocity;
        }

        if (pastVelX < 0) {
          std::cout << "d" << std::endl;

          guard.velX = 0;
          guard.velY = -defaultVelocity;
        }

        lastPositionChange = totalTime;
      }
    }

    auto pathElement = GameObject(
        1,
        1,
        guard.x + guard.width/2,
        guard.y + guard.height/2,
        0,
        0,
        {0, 255, 0}
    );

    // if path element out of bounds
    if (pathElement.x > boundaryWidth || pathElement.y > boundaryHeight) {
      guard.velX = 0;
      guard.velY = 0;

      std::cout << "Path total velocity: " << pathTotalVelocity << std::endl;
    } else {
      pathTotalVelocity += std::abs(guard.velX) + std::abs(guard.velY);
      path.push_back(pathElement);
    }
  }

  void OnRender(Core::Window& window, Core::Renderer& renderer) override {
    guard.Render(renderer);
    for (auto& obstruction : obstructions) {
      obstruction.Render(renderer);
    }
    for (auto& pathElement : path) {
      pathElement.Render(renderer);
    }

    SDL_Rect rect = {0, 0, boundaryWidth, boundaryHeight};
    SDL_SetRenderDrawColor(renderer.Get().get(), 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer.Get().get(), &rect);
  }
};
