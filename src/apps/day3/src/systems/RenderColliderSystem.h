#pragma once

#include <SDL.h>

#include "core/Renderer.h"
#include "ecs/Registry.h"
#include "ecs/System.h"

#include "../components/BoxColliderComponent.h"
#include "../components/RigidBodyComponent.h"

class RenderCollidersSystem : public ECS::System {
 public:
  RenderCollidersSystem() {
    RequireComponent<BoxColliderComponent>();
    RequireComponent<RigidBodyComponent>();
  }

  void Render(Core::Renderer& renderer) {
    const int borderWidth = 5;

    for (auto entity : GetSystemEntities()) {
      auto& boxCollider =
          ECS::Registry::Instance().GetComponent<BoxColliderComponent>(entity);
      auto& rigidBody =
          ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);

      SDL_SetRenderDrawColor(
          renderer.Get().get(),
          boxCollider.color.r,
          boxCollider.color.g,
          boxCollider.color.b,
          boxCollider.color.a
      );

      for (int i = 0; i < borderWidth; ++i) {
        SDL_Rect colliderRect = {
            static_cast<int>(rigidBody.position.x + boxCollider.offset.x - i),
            static_cast<int>(rigidBody.position.y + boxCollider.offset.y - i),
            boxCollider.width + i * 2,
            boxCollider.height + i * 2};
        SDL_RenderDrawRect(renderer.Get().get(), &colliderRect);
      }
    }

    SDL_SetRenderDrawColor(renderer.Get().get(), 0, 0, 0, 255);
  }
};
