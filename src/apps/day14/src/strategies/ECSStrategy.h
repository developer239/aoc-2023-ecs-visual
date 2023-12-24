#pragma once

#include <future>
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

std::string filePath = "assets/input.txt";

class MinimalLoopStrategy : public Core::IStrategy {
 public:
  float scale = 50.0f;
  int numRows = 0;
  int numCols = 0;

  bool alwaysShowSimulation = true;
  std::shared_ptr<bool> isTiltedNorth = std::make_shared<bool>(false);
  std::shared_ptr<bool> simulationStarted = std::make_shared<bool>(false);

  std::optional<ECS::Entity> tiltTrackerEntity;

  std::optional<ECS::Entity> cameraEntity;
  std::optional<ECS::Entity> puzzleSolverEntity1;
  std::optional<ECS::Entity> puzzleSolverEntity2;

  void Init(Core::Window& window, Core::Renderer& renderer) override {
    Core::AssetStore::Instance().AddFont("pico8", "assets/fonts/arial.ttf", 24);

    // Puzzle related entities
    auto inputData = ParseInput(filePath);
    auto platform = BuildPlatformFromInput(inputData);

    numRows = platform.tiles.size();
    numCols = platform.tiles[0].size();

    // Create UI entities for puzzle solver values
    tiltTrackerEntity = ECS::Registry::Instance().CreateEntity();
    ECS::Registry::Instance().AddComponent<RigidBodyComponent>(
        tiltTrackerEntity.value(),
        250,
        50,
        Vec2(550, 50),
        true,
        SDL_Color{0, 0, 255, 255},
        true
    );

    auto initialLabel = std::string("Default state");
    ECS::Registry::Instance().AddComponent<TextComponent>(
        tiltTrackerEntity.value(),
        initialLabel,
        SDL_Color{255, 255, 255, 255},
        24,
        true
    );
    // UI end

    // Systems
    ECS::Registry::Instance().AddSystem<RenderRigidBodiesSystem>();
    ECS::Registry::Instance().AddSystem<KeyboardControlSystem>();
    ECS::Registry::Instance().AddSystem<CollisionSystem>(scale);
    ECS::Registry::Instance().AddSystem<RenderTextSystem>();
    ECS::Registry::Instance().AddSystem<RenderCollidersSystem>();
    ECS::Registry::Instance().AddSystem<PuzzleSolverSystem>(
        isTiltedNorth,
        tiltTrackerEntity,
        simulationStarted,
        scale,
        numRows,
        numCols
    );
    ECS::Registry::Instance().AddSystem<CameraSystem>();

    // Events
    ECS::Registry::Instance()
        .GetSystem<KeyboardControlSystem>()
        .SubscribeToEvents();

    ECS::Registry::Instance().GetSystem<PuzzleSolverSystem>().SubscribeToEvents(
    );
    ECS::Registry::Instance().GetSystem<CameraSystem>().SubscribeToEvents();

    // Entities & Components

    // Fixed cell size
    const float cellSize = scale;

    // Initialize grid system
    ECS::Registry::Instance().AddSystem<RenderGridSystem>(cellSize, cellSize);

    auto cubeShapedRocks = platform.GetTiles(TileType::CUBE_SHAPED_ROCK);
    for (auto& cubeShapedRock : cubeShapedRocks) {
      ECS::Entity cubeShapedRockEntity =
          ECS::Registry::Instance().CreateEntity();
      ECS::Registry::Instance().GroupEntity(
          cubeShapedRockEntity,
          "CubeShapedRock"
      );

      int scaledX = cubeShapedRock.col * cellSize;
      int scaledY = cubeShapedRock.row * cellSize;
      int scaledWidth = cellSize;
      int scaledHeight = cellSize;

      ECS::Registry::Instance().AddComponent<RigidBodyComponent>(
          cubeShapedRockEntity,
          scaledWidth,
          scaledHeight,
          Vec2(scaledX, scaledY),
          true
      );

      ECS::Registry::Instance().AddComponent<BoxColliderComponent>(
          cubeShapedRockEntity,
          scaledWidth - 4,
          scaledHeight - 2,
          Vec2(2, 1),
          SDL_Color{0, 0, 255, 0}
      );

      ECS::Registry::Instance().AddComponent<TextComponent>(
          cubeShapedRockEntity,
          "#",
          SDL_Color{0, 0, 255, 255},
          6
      );
    }

    auto roundedShapedRocks = platform.GetTiles(TileType::ROUNDED_ROCK);
    for (auto& roundedShapedRock : roundedShapedRocks) {
      ECS::Entity cubeShapedRockEntity =
          ECS::Registry::Instance().CreateEntity();
      ECS::Registry::Instance().GroupEntity(
          cubeShapedRockEntity,
          "RoundedShapedRock"
      );

      int scaledX = roundedShapedRock.col * cellSize;
      int scaledY = roundedShapedRock.row * cellSize;
      int scaledWidth = cellSize;
      int scaledHeight = cellSize;

      ECS::Registry::Instance().AddComponent<RigidBodyComponent>(
          cubeShapedRockEntity,
          scaledWidth,
          scaledHeight,
          Vec2(scaledX, scaledY),
          true
      );

      ECS::Registry::Instance().AddComponent<BoxColliderComponent>(
          cubeShapedRockEntity,
          scaledWidth - 8,
          scaledHeight,
          Vec2(4, 0),
          SDL_Color{0, 0, 255, 0}
      );

      ECS::Registry::Instance().AddComponent<TextComponent>(
          cubeShapedRockEntity,
          "O",
          SDL_Color{0, 0, 255, 255},
          6
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
  }

  void HandleEvent(SDL_Event& event) override {
    Events::Bus::Instance().EmitEvent<KeyPressedEvent>(event.key.keysym.sym);
  }

  void OnUpdate(
      Core::Window& window, Core::Renderer& renderer, double deltaTime
  ) override {

    for (int column = 0; column < numCols; column++) {
      auto future = std::async(
          std::launch::async,
          &CollisionSystem::Update,
          &ECS::Registry::Instance().GetSystem<CollisionSystem>(),
          column
      );
    }

    ECS::Registry::Instance().GetSystem<PuzzleSolverSystem>().Update();

    ECS::Registry::Instance().Update();
  }

  void OnRender(Core::Window& window, Core::Renderer& renderer) override {
    ECS::Registry::Instance().GetSystem<RenderGridSystem>().Render(
        renderer,
        window
    );

    if (!*simulationStarted || alwaysShowSimulation) {
      ECS::Registry::Instance().GetSystem<RenderCollidersSystem>().Render(
          renderer,
          cameraEntity.value()
      );

      ECS::Registry::Instance().GetSystem<RenderRigidBodiesSystem>().Render(
          renderer,
          cameraEntity.value()
      );
      ECS::Registry::Instance().GetSystem<RenderTextSystem>().Render(
          renderer,
          cameraEntity.value()
      );
    }
  }
};
