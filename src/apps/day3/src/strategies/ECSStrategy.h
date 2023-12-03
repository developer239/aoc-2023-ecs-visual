#pragma once

#include <optional>

#include "core/AssetStore.h"
#include "core/IStrategy.h"
#include "core/Window.h"
#include "ecs/Registry.h"

#include "../common/Vec2.h"
#include "../components/TextComponent.h"
#include "../services/PuzzleInputParser.h"
#include "../systems/CameraSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/KeyboardControlSystem.h"
#include "../systems/PuzzleSolverSystem.h"
#include "../systems/RenderColliderSystem.h"
#include "../systems/RenderGridSystem.h"
#include "../systems/RenderRigidBodiesSystem.h"
#include "../systems/RenderTextSystem.h"

class MinimalLoopStrategy : public Core::IStrategy {
 public:
  std::optional<ECS::Entity> cameraEntity;
  std::optional<ECS::Entity> puzzleSolverEntity1;
  std::optional<ECS::Entity> puzzleSolverEntity2;

  void Init(Core::Window& window, Core::Renderer& renderer) override {
    Core::AssetStore::Instance().AddFont("pico8", "assets/fonts/arial.ttf", 24);

    // Systems
    ECS::Registry::Instance().AddSystem<RenderRigidBodiesSystem>();
    ECS::Registry::Instance().AddSystem<KeyboardControlSystem>();
    ECS::Registry::Instance().AddSystem<CollisionSystem>();
    ECS::Registry::Instance().AddSystem<RenderTextSystem>();
    ECS::Registry::Instance().AddSystem<RenderCollidersSystem>();
    ECS::Registry::Instance().AddSystem<PuzzleSolverSystem>();
    ECS::Registry::Instance().AddSystem<CameraSystem>();

    // Events
    ECS::Registry::Instance()
        .GetSystem<KeyboardControlSystem>()
        .SubscribeToEvents();

    ECS::Registry::Instance().GetSystem<PuzzleSolverSystem>().SubscribeToEvents(
    );
    ECS::Registry::Instance().GetSystem<CameraSystem>().SubscribeToEvents(
    );

    // Entities & Components

    // Puzzle related entities
    auto inputData = ParseInput("assets/input-example-1.txt");
    auto partsAndSymbols = FindPartsAndSymbols(inputData);

    // Fixed cell size
    const float cellSize = 50.0f;

    // Initialize grid system
    ECS::Registry::Instance().AddSystem<RenderGridSystem>(
        cellSize,
        cellSize
    );

    // Create entities for parts
    for (const auto& part : partsAndSymbols.parts) {
      ECS::Entity partEntity = ECS::Registry::Instance().CreateEntity();
      ECS::Registry::Instance().TagEntity(partEntity, "EnginePart");

      int scaledX = part.column * cellSize;
      int scaledY = part.row * cellSize;
      int scaledWidth = part.number.length() * cellSize;
      int scaledHeight = cellSize;

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

      int scaledX = symbol.column * cellSize;
      int scaledY = symbol.row * cellSize;
      int scaledSize = cellSize; // Assuming square size for symbols

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

    // Camera
    cameraEntity = ECS::Registry::Instance().CreateEntity();
    ECS::Registry::Instance().AddComponent<CameraComponent>(
        cameraEntity.value(),
        Vec2(0, 0),
        window.GetWidth(),
        window.GetHeight()
    );

    // Results

    // Create UI entities for puzzle solver values
    puzzleSolverEntity1 = ECS::Registry::Instance().CreateEntity();
    puzzleSolverEntity2 = ECS::Registry::Instance().CreateEntity();

    // Set up the first UI entity
    ECS::Registry::Instance().AddComponent<RigidBodyComponent>(
        puzzleSolverEntity1.value(),
        300, 50,
        Vec2(500, 50),
        true,
        SDL_Color{0, 0, 255, 255},
        true
    );
    ECS::Registry::Instance().AddComponent<TextComponent>(
        puzzleSolverEntity1.value(),
        "Puzzle Solver Value 1",
        SDL_Color{255, 255, 255, 255},
        24,
        true
    );

    ECS::Registry::Instance().AddComponent<RigidBodyComponent>(
        puzzleSolverEntity2.value(),
        300, 50,
        Vec2(500, 100),
        true,
        SDL_Color{0, 0, 255, 255},
        true
    );
    ECS::Registry::Instance().AddComponent<TextComponent>(
        puzzleSolverEntity2.value(),
        "Puzzle Solver Value 2",
        SDL_Color{255, 255, 255, 255},
        24,
        true
    );
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
        .CalculateSumOfAllParts(puzzleSolverEntity1.value());
    ECS::Registry::Instance()
        .GetSystem<PuzzleSolverSystem>()
        .CalculateSumAllGearRatios(puzzleSolverEntity2.value());
  }

  void OnRender(Core::Window& window, Core::Renderer& renderer) override {
    ECS::Registry::Instance().GetSystem<RenderGridSystem>().Render(
        renderer,
        window
    );

    ECS::Registry::Instance().GetSystem<RenderCollidersSystem>().Render(renderer, cameraEntity.value());

    ECS::Registry::Instance().GetSystem<RenderRigidBodiesSystem>().Render(
        renderer,
        cameraEntity.value()
    );
    ECS::Registry::Instance().GetSystem<RenderTextSystem>().Render(renderer, cameraEntity.value());

  }
};
