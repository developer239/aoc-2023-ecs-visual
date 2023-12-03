#pragma once

#include "core/AssetStore.h"
#include "core/IStrategy.h"
#include "core/Window.h"
#include "ecs/Registry.h"

#include "../common/Vec2.h"
#include "../components/TextComponent.h"
#include "../services/PuzzleInputParser.h"
#include "../systems/CollisionSystem.h"
#include "../systems/KeyboardControlSystem.h"
#include "../systems/PuzzleSolverSystem.h"
#include "../systems/RenderColliderSystem.h"
#include "../systems/RenderGridSystem.h"
#include "../systems/RenderRigidBodiesSystem.h"
#include "../systems/RenderTextSystem.h"

class MinimalLoopStrategy : public Core::IStrategy {
 public:
  void Init(Core::Window& window, Core::Renderer& renderer) override {
    Core::AssetStore::Instance().AddFont("pico8", "assets/fonts/arial.ttf", 24);

    // Systems
    ECS::Registry::Instance().AddSystem<RenderRigidBodiesSystem>();
    ECS::Registry::Instance().AddSystem<KeyboardControlSystem>();
    ECS::Registry::Instance().AddSystem<CollisionSystem>();
    ECS::Registry::Instance().AddSystem<RenderTextSystem>();
    ECS::Registry::Instance().AddSystem<RenderCollidersSystem>();
    ECS::Registry::Instance().AddSystem<PuzzleSolverSystem>();

    // Events
    ECS::Registry::Instance()
        .GetSystem<KeyboardControlSystem>()
        .SubscribeToEvents();

    ECS::Registry::Instance().GetSystem<PuzzleSolverSystem>().SubscribeToEvents(
    );

    // Entities & Components

    auto inputData = ParseInput("assets/input-example-1.txt");
    auto partsAndSymbols = FindPartsAndSymbols(inputData);

    int maxWidth = 0;
    for (const auto& line : inputData) {
      maxWidth = std::max(maxWidth, static_cast<int>(line.length()));
    }
    int maxHeight = inputData.size();

    // Calculate a uniform scale factor for both width and height
    float uniformScale = std::min(
        static_cast<float>(window.GetWidth()) / maxWidth,
        static_cast<float>(window.GetHeight()) / maxHeight
    );

    // Initialize grid system
    ECS::Registry::Instance().AddSystem<RenderGridSystem>(
        uniformScale,
        uniformScale
    );

    // Create entities for parts
    for (const auto& part : partsAndSymbols.parts) {
      ECS::Entity partEntity = ECS::Registry::Instance().CreateEntity();
      ECS::Registry::Instance().TagEntity(partEntity, "EnginePart");

      // Calculate the scaled position and size, round to the nearest integer
      int scaledX = std::lround(part.column * uniformScale);
      int scaledY = std::lround(part.row * uniformScale);
      int scaledWidth = std::lround(part.number.length() * uniformScale);
      int scaledHeight =
          std::lround(uniformScale);  // Height is based on a single character

      ECS::Registry::Instance().AddComponent<RigidBodyComponent>(
          partEntity,
          scaledWidth,
          scaledHeight,
          Vec2(scaledX, scaledY),
          true
      );

      ECS::Registry::Instance().AddComponent<TextComponent>(
          partEntity,
          part.number
      );

      ECS::Registry::Instance().AddComponent<BoxColliderComponent>(
          partEntity,
          scaledWidth,
          scaledHeight,
          Vec2(0, 0)
      );
    }

    // Create entities for symbols
    for (const auto& symbol : partsAndSymbols.symbols) {
      ECS::Entity symbolEntity = ECS::Registry::Instance().CreateEntity();
      ECS::Registry::Instance().TagEntity(symbolEntity, "Symbol");

      // Calculate the scaled position and size, round to the nearest integer
      int scaledX = std::lround(symbol.column * uniformScale);
      int scaledY = std::lround(symbol.row * uniformScale);
      int scaledSize =
          std::lround(uniformScale);  // Assuming square size for symbols

      ECS::Registry::Instance().AddComponent<RigidBodyComponent>(
          symbolEntity,
          scaledSize,
          scaledSize,
          Vec2(scaledX, scaledY),
          true
      );

      ECS::Registry::Instance().AddComponent<TextComponent>(
          symbolEntity,
          symbol.symbol
      );

      auto offsetRatio = 5;
      auto bboxOffset = -scaledSize / offsetRatio;

      ECS::Registry::Instance().AddComponent<BoxColliderComponent>(
          symbolEntity,
          scaledSize + (-1 * bboxOffset) * 2,
          scaledSize + (-1 * bboxOffset) * 2,
          Vec2(bboxOffset, bboxOffset)
      );
    }
  }

  void HandleEvent(SDL_Event& event) override {
    Events::Bus::Instance().EmitEvent<KeyPressedEvent>(event.key.keysym.sym);
  }

  void OnUpdate(
      Core::Window& window, Core::Renderer& renderer, double deltaTime
  ) override {
    ECS::Registry::Instance().GetSystem<CollisionSystem>().Update();

    ECS::Registry::Instance().Update();

    ECS::Registry::Instance()
        .GetSystem<PuzzleSolverSystem>()
        .CalculateSumOfAllParts();
    ECS::Registry::Instance()
        .GetSystem<PuzzleSolverSystem>()
        .CalculateSumAllGearRatios();
  }

  void OnRender(Core::Window& window, Core::Renderer& renderer) override {
    ECS::Registry::Instance().GetSystem<RenderGridSystem>().Render(
        renderer,
        window
    );
    ECS::Registry::Instance().GetSystem<RenderRigidBodiesSystem>().Render(
        renderer
    );
    ECS::Registry::Instance().GetSystem<RenderTextSystem>().Render(renderer);

    ECS::Registry::Instance().GetSystem<RenderCollidersSystem>().Render(renderer
    );
  }
};
