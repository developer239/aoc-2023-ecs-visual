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
    for (auto entity : GetSystemEntities()) {
      auto& boxCollider =
          ECS::Registry::Instance().GetComponent<BoxColliderComponent>(entity);
      auto& rigidBody =
          ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);

      SDL_Rect colliderRect = {
          static_cast<int>(rigidBody.position.x + boxCollider.offset.x),
          static_cast<int>(rigidBody.position.y + boxCollider.offset.y),
          boxCollider.width,
          boxCollider.height};

      SDL_SetRenderDrawColor(
          renderer.Get().get(),
          boxCollider.color.r,
          boxCollider.color.g,
          boxCollider.color.b,
          boxCollider.color.a
      );
      SDL_RenderDrawRect(renderer.Get().get(), &colliderRect);
    }

    // Reset the renderer color to avoid affecting other render calls
    SDL_SetRenderDrawColor(
        renderer.Get().get(),
        0,
        0,
        0,
        255
    );  // Back to black or another default color
  }
};
