#pragma once

#include "core/Loop.h"
#include "ecs/System.h"
#include "events/Bus.h"
#include "../events/CollisionEvent.h"
#include "../components/RigidBodyComponent.h"

class MovementSystem : public ECS::System {
 public:
  MovementSystem() {
    RequireComponent<RigidBodyComponent>();
  }

  void SubscribeToEvents() {
    Events::Bus::Instance().SubscribeToEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
  }

  void OnCollision(CollisionEvent& event) {
    ECS::Entity a = event.a;
    ECS::Entity b = event.b;

    std::unique_ptr<ECS::Entity> ball;

    if (ECS::Registry::Instance().DoesEntityHaveTag(a, "Ball")) {
      ball = std::make_unique<ECS::Entity>(a);
    }

    if (ECS::Registry::Instance().DoesEntityHaveTag(b, "Ball")) {
      ball = std::make_unique<ECS::Entity>(b);
    }

    auto& rigidBodyComponent = ECS::Registry::Instance().GetComponent<RigidBodyComponent>(*ball);
    rigidBodyComponent.velocity.y = -rigidBodyComponent.velocity.y;
  }

  void Update(double deltaTime, Core::Window& window) {
    for (auto entity: GetSystemEntities()) {
      auto& rigidBodyComponent = ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);

      // update entity position

      rigidBodyComponent.position.x += rigidBodyComponent.velocity.x * deltaTime;
      rigidBodyComponent.position.y += rigidBodyComponent.velocity.y * deltaTime;

      // fix entity position if it is out of bounds

      if (rigidBodyComponent.position.x < 0) {
        rigidBodyComponent.position.x = 0;

        if (ECS::Registry::Instance().DoesEntityHaveTag(entity, "Ball")) {
          rigidBodyComponent.velocity.x = -rigidBodyComponent.velocity.x;
        } else {
          rigidBodyComponent.velocity.x = 0;
        }
      }

      if (rigidBodyComponent.position.x + rigidBodyComponent.width > window.GetWidth()) {
        rigidBodyComponent.position.x = window.GetWidth() - rigidBodyComponent.width;

        if (ECS::Registry::Instance().DoesEntityHaveTag(entity, "Ball")) {
          rigidBodyComponent.velocity.x = -rigidBodyComponent.velocity.x;
        } else {
          rigidBodyComponent.velocity.x = 0;
        }
      }

      if (rigidBodyComponent.position.y < 0) {
        rigidBodyComponent.position.y = 0;

        if (ECS::Registry::Instance().DoesEntityHaveTag(entity, "Ball")) {
          rigidBodyComponent.velocity.y = -rigidBodyComponent.velocity.y;
        } else {
          rigidBodyComponent.velocity.y = 0;
        }
      }

      // TODO: this should probably live in a different system
      // check if game over
      if (rigidBodyComponent.position.y + rigidBodyComponent.height > window.GetHeight()) {
        if (ECS::Registry::Instance().DoesEntityHaveTag(entity, "Ball")) {
          rigidBodyComponent.position.x = window.GetWidth() / 2;
          rigidBodyComponent.position.y = 0;
        }
      }
    }
  }
};
