#pragma once

#include "../components/CameraComponent.h"
#include "../events/KeyPressedEvent.h"
#include "ecs/System.h"
#include "events/Bus.h"

class CameraSystem : public ECS::System {
 public:
  CameraSystem() {
    RequireComponent<CameraComponent>();
    SubscribeToEvents();
  }

  void SubscribeToEvents() {
    Events::Bus::Instance().SubscribeToEvent<KeyPressedEvent>(
        this,
        &CameraSystem::OnKeyPressed
    );
  }

  void OnKeyPressed(KeyPressedEvent& event) {
    for (auto entity : GetSystemEntities()) {
      auto& camera =
          ECS::Registry::Instance().GetComponent<CameraComponent>(entity);

      int cameraSpeed = 10;

      switch (event.symbol) {
        case SDLK_UP:
          camera.position.y -= cameraSpeed;
          break;
        case SDLK_DOWN:
          camera.position.y += cameraSpeed;
          break;
        case SDLK_LEFT:
          camera.position.x -= cameraSpeed;
          break;
        case SDLK_RIGHT:
          camera.position.x += cameraSpeed;
          break;
      }
    }
  }

  void Update() {
    // Additional logic for updating camera (if needed)
  }
};
