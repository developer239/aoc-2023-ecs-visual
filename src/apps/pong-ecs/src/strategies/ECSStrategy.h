#pragma once

#include "core/AssetStore.h"
#include "core/IStrategy.h"
#include "core/Window.h"
#include "ecs/Registry.h"

#include "../common/Vec2.h"
#include "../systems/CollisionSystem.h"
#include "../systems/KeyboardControlSystem.h"
#include "../systems/MovementSystem.h"
#include "../systems/RenderRigidBodiesSystem.h"

class MinimalLoopStrategy : public Core::IStrategy {
 public:
  void Init(Core::Window& window, Core::Renderer& renderer) override {
    Core::AssetStore::Instance().AddFont("pico8", "assets/fonts/pico8.ttf", 24);

    // Systems

    ECS::Registry::Instance().AddSystem<MovementSystem>();
    ECS::Registry::Instance().AddSystem<RenderRigidBodiesSystem>();
    ECS::Registry::Instance().AddSystem<KeyboardControlSystem>();
    ECS::Registry::Instance().AddSystem<CollisionSystem>();

    // Components

    ECS::Entity ball = ECS::Registry::Instance().CreateEntity();
    ECS::Registry::Instance().TagEntity(ball, "Ball");
    ECS::Registry::Instance()
        .AddComponent<BoxColliderComponent>(ball, 15, 15, Vec2(0, 0));
    ECS::Registry::Instance().AddComponent<RigidBodyComponent>(
        ball,
        15,
        15,
        Vec2(20.0, 20.0),
        Vec2(300.0, 300.0)
    );

    ECS::Entity paddle = ECS::Registry::Instance().CreateEntity();
    ECS::Registry::Instance().TagEntity(paddle, "Paddle");
    ECS::Registry::Instance()
        .AddComponent<BoxColliderComponent>(paddle, 100, 20, Vec2(0, 0));
    ECS::Registry::Instance().AddComponent<KeyboardControlledComponent>(
        paddle,
        Vec2(0, 0),
        Vec2(400, 0),
        Vec2(0, 0),
        Vec2(-400, 0)
    );
    ECS::Registry::Instance().AddComponent<RigidBodyComponent>(
        paddle,
        100,
        20,
        Vec2((window.GetWidth() / 2) - (100 / 2), window.GetHeight() - 40),
        Vec2(0, 0)
    );

    // Events

    ECS::Registry::Instance()
        .GetSystem<KeyboardControlSystem>()
        .SubscribeToEvents();
    ECS::Registry::Instance().GetSystem<MovementSystem>().SubscribeToEvents();
  }

  void HandleEvent(SDL_Event& event) override {
    Events::Bus::Instance().EmitEvent<KeyPressedEvent>(event.key.keysym.sym);
  }

  void OnUpdate(
      Core::Window& window, Core::Renderer& renderer, double deltaTime
  ) override {
    ECS::Registry::Instance().GetSystem<MovementSystem>().Update(deltaTime, window);
    ECS::Registry::Instance().GetSystem<CollisionSystem>().Update();

    ECS::Registry::Instance().Update();
  }

  void OnRender(Core::Window& window, Core::Renderer& renderer) override {
    ECS::Registry::Instance().GetSystem<RenderRigidBodiesSystem>().Render(
        renderer
    );
  }
};
