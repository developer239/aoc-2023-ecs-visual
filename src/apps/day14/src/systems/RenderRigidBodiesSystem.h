#pragma once

#include <SDL.h>
#include "ecs/System.h"
#include "events/Bus.h"

#include "../components/RigidBodyComponent.h"
#include "../components/CameraComponent.h"

class RenderRigidBodiesSystem : public ECS::System {
 public:
  RenderRigidBodiesSystem() { RequireComponent<RigidBodyComponent>(); }

  void Render(Core::Renderer& renderer, ECS::Entity cameraEntity) {
    auto& camera = ECS::Registry::Instance().GetComponent<CameraComponent>(cameraEntity);

    for (auto entity : GetSystemEntities()) {
      auto& rigidBodyComponent = ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);

      // Determine position based on whether the entity is fixed or not
      int renderX, renderY;
      if (rigidBodyComponent.isFixed) {
        renderX = rigidBodyComponent.position.x;
        renderY = rigidBodyComponent.position.y;
      } else {
        renderX = static_cast<int>(rigidBodyComponent.position.x - camera.position.x);
        renderY = static_cast<int>(rigidBodyComponent.position.y - camera.position.y);
      }

      // Render only if the entity is within the camera's view (for non-fixed entities)
      if (rigidBodyComponent.isFixed || IsEntityInView(rigidBodyComponent, camera)) {
        SDL_Rect rect = { renderX, renderY, rigidBodyComponent.width, rigidBodyComponent.height };
        SDL_SetRenderDrawColor(renderer.Get().get(), rigidBodyComponent.color.r, rigidBodyComponent.color.g, rigidBodyComponent.color.b, rigidBodyComponent.color.a);
        SDL_RenderFillRect(renderer.Get().get(), &rect);
      }
    }
  }

 private:
  bool IsEntityInView(const RigidBodyComponent& rigidBody, const CameraComponent& camera) {
    // Check if the entity's bounds intersect with the camera's view
    bool isHorizontalInView = (rigidBody.position.x + rigidBody.width > camera.position.x) &&
                              (rigidBody.position.x < camera.position.x + camera.width);
    bool isVerticalInView = (rigidBody.position.y + rigidBody.height > camera.position.y) &&
                            (rigidBody.position.y < camera.position.y + camera.height);
    return isHorizontalInView && isVerticalInView;
  }
};
