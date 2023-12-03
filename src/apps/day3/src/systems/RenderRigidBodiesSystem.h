#pragma once

#include <SDL.h>
#include "ecs/System.h"
#include "events/Bus.h"

#include "../components/RigidBodyComponent.h"

class RenderRigidBodiesSystem : public ECS::System {
 public:
  RenderRigidBodiesSystem() { RequireComponent<RigidBodyComponent>(); }

  void Render(Core::Renderer& renderer) {
    for (auto entity : GetSystemEntities()) {
      auto rigidBodyComponent = ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);

      SDL_Rect rect = {
          (int)rigidBodyComponent.position.x,
          (int)rigidBodyComponent.position.y,
          (int)rigidBodyComponent.width,
          (int)rigidBodyComponent.height};
      SDL_SetRenderDrawColor(renderer.Get().get(), 255, 255, 255, 255);
      SDL_RenderFillRect(renderer.Get().get(), &rect);
    }
  }
};
