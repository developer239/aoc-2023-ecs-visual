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
  PuzzleSolverSystem() { RequireComponent<TextComponent>(); }

  void SubscribeToEvents() {
    Events::Bus::Instance().SubscribeToEvent<CollisionEvent>(
        this,
        &PuzzleSolverSystem::OnCollision
    );
  }

  void OnCollision(CollisionEvent& event) {
    auto& registry = ECS::Registry::Instance();
    bool isAGearPart = registry.DoesEntityHaveTag(event.a, "GearPart");
    bool isBGearPart = registry.DoesEntityHaveTag(event.b, "GearPart");
    bool isASymbol = registry.DoesEntityHaveTag(event.a, "Symbol");
    bool isBSymbol = registry.DoesEntityHaveTag(event.b, "Symbol");

    // Ensure one is a gear part and the other is a symbol
    if ((isAGearPart && isBSymbol) || (isBGearPart && isASymbol)) {
      ECS::Entity gearPartEntity = isAGearPart ? event.a : event.b;
      partEntities.insert(
          gearPartEntity
      );  // Insert the gear part

      auto& rigidBody = registry.GetComponent<RigidBodyComponent>(gearPartEntity);
      rigidBody.color = {50, 255, 50, 255};
    }
  }

  void CalculateSumAndPrint() {
    sumOfParts = 0;  // Reset sum before calculation

    // Iterate over all the part entities that have collided with symbols
    for (auto& entity : partEntities) {
      if (ECS::Registry::Instance().HasComponent<TextComponent>(entity)) {
        auto& textComponent =
            ECS::Registry::Instance().GetComponent<TextComponent>(entity);
        sumOfParts += std::stoi(textComponent.text
        );  // Convert text to integer and add to sum
      }
    }

    // Print out the result
    std::cout << "The sum of all part numbers is: " << sumOfParts << std::endl;

    // Clear the set for the next calculation
    partEntities.clear();
  }

 private:
  std::unordered_set<ECS::Entity> partEntities;
  int sumOfParts = 0;
};
