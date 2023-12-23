#pragma once

#include "ecs/System.h"
#include "events/Bus.h"

#include "../components/KeyboardControlledComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../events/CollisionEvent.h"
#include "../events/KeyPressedEvent.h"

class KeyboardControlSystem : public ECS::System {
 public:
  KeyboardControlSystem() { RequireComponent<KeyboardControlledComponent>(); }

  void SubscribeToEvents() {
    Events::Bus::Instance().SubscribeToEvent<KeyPressedEvent>(
        this,
        &KeyboardControlSystem::OnKeyPressed
    );
  }

  void OnKeyPressed(KeyPressedEvent& event) {
    for (auto entity : GetSystemEntities()) {
    }
  }

  void Update() {}
};
