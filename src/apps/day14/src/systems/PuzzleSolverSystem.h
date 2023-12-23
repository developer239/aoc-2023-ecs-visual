#pragma once

#include <iostream>
#include <string>
#include <unordered_set>

#include "ecs/Registry.h"
#include "ecs/System.h"
#include "events/Bus.h"

namespace ECS {
class Entity;  // Forward declaration if needed
}

// This should be in a separate header or in a commonly included header
// after the ECS::Entity class definition
namespace std {
template <>
struct hash<ECS::Entity> {
  std::size_t operator()(const ECS::Entity& entity) const noexcept {
    return std::hash<int>()(entity.GetId());
  }
};
}  // namespace std

class PuzzleSolverSystem : public ECS::System {
 public:
  PuzzleSolverSystem(
      std::shared_ptr<bool> isTiltedNorth,
      std::optional<ECS::Entity>& tiltTrackerEntity,
      std::shared_ptr<bool> simulationStarted
  )
      : isTiltedNorth(isTiltedNorth),
        tiltTrackerEntity(tiltTrackerEntity),
        simulationStarted(simulationStarted) {
    RequireComponent<TextComponent>();
  }

  void SubscribeToEvents() {
    Events::Bus::Instance().SubscribeToEvent<KeyPressedEvent>(
        this,
        &PuzzleSolverSystem::OnKeyPressed
    );

    Events::Bus::Instance().SubscribeToEvent<CollisionEvent>(
        this,
        &PuzzleSolverSystem::OnCollision
    );
  }

  void OnCollision(CollisionEvent& event) {}

  void Update() {
    //
    // Move all entities (should be a specific system)
    auto entities = GetSystemEntities();
    for (auto entity : entities) {
      auto& rigidBody =
          ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);
      rigidBody.position.x += rigidBody.velocity.x;
      rigidBody.position.y += rigidBody.velocity.y;
    }

    //
    // resolve simulation status
    bool isAnyMovingUp = false;
    for (auto entity : entities) {
      auto& rigidBody =
          ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);

      if (rigidBody.velocity.y < 0) {
        isAnyMovingUp = true;
        break;
      }
    }

    if (!isAnyMovingUp) {
      *simulationStarted = false;

      auto& tiltTrackerTextComponent =
          ECS::Registry::Instance().GetComponent<TextComponent>(
              tiltTrackerEntity.value()
          );
      tiltTrackerTextComponent.text = "Ended: North";
    }

    // calculate result
    //    The amount of load caused by a single rounded rock (O) is equal to the
    //    number of rows from the rock to the south edge of the platform,
    //    including the row the rock is on. (Cube-shaped rocks (#) don't
    //    contribute to load.) So, the amount of load caused by each rock in
    //    each row is as follows:
    if (*simulationStarted) return;

    const float SCALE = 25.0f;
    int HEIGHT = 100 + 1;

    auto entitiesByGroup =
        ECS::Registry::Instance().GetEntitiesByGroup("RoundedShapedRock");
    std::unordered_set<ECS::Entity> entitiesSet(
        entitiesByGroup.begin(),
        entitiesByGroup.end()
    );
    int totalLoad = 0;
    for (auto entity : entitiesSet) {
      auto& rigidBody =
          ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);
      auto totalScaledDiff = (rigidBody.position.y) + SCALE;

      auto& textComponent =
          ECS::Registry::Instance().GetComponent<TextComponent>(entity);

      auto totalActualDiff = totalScaledDiff / SCALE;

      auto totalNormalizedDiff = -totalActualDiff + HEIGHT;

      int totalRoundedDiff = std::round(totalNormalizedDiff);

      std::cout << "Total rounded diff: " << totalRoundedDiff << std::endl;
      totalLoad += totalRoundedDiff;

      textComponent.text = std::string("O ") + std::to_string(totalRoundedDiff);
    }

    std::cout << "Total load: " << totalLoad << std::endl;
  }

  void OnKeyPressed(KeyPressedEvent& event) {
    switch (event.symbol) {
      case SDLK_n:
        StartSimulation();
        break;
    }
  }

 private:
  void StartSimulation() {
    auto& tiltTrackerTextComponent =
        ECS::Registry::Instance().GetComponent<TextComponent>(
            tiltTrackerEntity.value()
        );
    tiltTrackerTextComponent.text = "Started: North";

    *isTiltedNorth = true;
    *simulationStarted = true;

    auto entities =
        ECS::Registry::Instance().GetEntitiesByGroup("RoundedShapedRock");
    for (auto entity : entities) {
      auto& rigidBody =
          ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);
      rigidBody.velocity.y = -1;
    }
  }

  std::shared_ptr<bool> simulationStarted;
  std::shared_ptr<bool> isTiltedNorth;
  std::optional<ECS::Entity> tiltTrackerEntity;
};
