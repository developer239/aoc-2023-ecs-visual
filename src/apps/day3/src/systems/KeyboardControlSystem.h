#pragma once

#include "ecs/System.h"
#include "events/Bus.h"

#include "../events/CollisionEvent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/KeyboardControlledComponent.h"
#include "../events/KeyPressedEvent.h"

class KeyboardControlSystem : public ECS::System {
 public:
  KeyboardControlSystem() {
    RequireComponent<KeyboardControlledComponent>();
  }

  void SubscribeToEvents() {
    Events::Bus::Instance().SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
  }

  void OnKeyPressed(KeyPressedEvent& event) {
    for (auto entity: GetSystemEntities()) {
      const auto keyboardControl = ECS::Registry::Instance().GetComponent<KeyboardControlledComponent>(entity);
      auto& rigidBody = ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);

      switch (event.symbol) {
        case SDLK_UP:
          rigidBody.velocity = keyboardControl.upVelocity;
          break;
        case SDLK_RIGHT:
          rigidBody.velocity = keyboardControl.rightVelocity;
          break;
        case SDLK_DOWN:
          rigidBody.velocity = keyboardControl.downVelocity;
          break;
        case SDLK_LEFT:
          rigidBody.velocity = keyboardControl.leftVelocity;
          break;
      }
    }
  }

  void Update() {
  }
};
