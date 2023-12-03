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
    bool isAEnginePart = registry.DoesEntityHaveTag(event.a, "EnginePart");
    bool isBEnginePart = registry.DoesEntityHaveTag(event.b, "EnginePart");
    bool isASymbol = registry.DoesEntityHaveTag(event.a, "Symbol");
    bool isBSymbol = registry.DoesEntityHaveTag(event.b, "Symbol");

    // Ensure one is a engine part and the other is a symbol
    if ((isAEnginePart && isBSymbol) || (isBEnginePart && isASymbol)) {
      ECS::Entity enginePartEntity = isAEnginePart ? event.a : event.b;
      ECS::Entity symbolEntity = isASymbol ? event.a : event.b;

      partEntities.insert(enginePartEntity);  // Insert the engine part

      auto& rigidBody =
          registry.GetComponent<RigidBodyComponent>(enginePartEntity);
      rigidBody.color = {50, 255, 50, 255};

      auto enginePartTextComponent =
          registry.GetComponent<TextComponent>(enginePartEntity);

      auto symbolTextComponent =
          registry.GetComponent<TextComponent>(symbolEntity);
      auto symbolRigidBody =
          registry.GetComponent<RigidBodyComponent>(symbolEntity);

      if (symbolTextComponent.text == "*") {
        std::string key = std::to_string(symbolRigidBody.position.x) + "-" +
                          std::to_string(symbolRigidBody.position.y);
        auto& gearSymbol = gearSymbolsMap[key];

        if (std::find(
                gearSymbol.adjacentNumbers.begin(),
                gearSymbol.adjacentNumbers.end(),
                enginePartTextComponent.text
            ) == gearSymbol.adjacentNumbers.end()) {
          gearSymbol.adjacentCount++;
          gearSymbol.adjacentNumbers.push_back(enginePartTextComponent.text);
        }
      }
    }
  }

  void CalculateSumAllGearRatios() {
    int sumOfAllGearRatios = 0;
    for (auto& [key, gearSymbol] : gearSymbolsMap) {
      if (gearSymbol.adjacentCount == 2) {
        auto gearRatio = std::stoi(gearSymbol.adjacentNumbers[0]) *
                         std::stoi(gearSymbol.adjacentNumbers[1]);
        sumOfAllGearRatios += gearRatio;
      }
    }

    std::cout << "The sum of all gear ratios is: " << sumOfAllGearRatios
              << std::endl;
  }

  void CalculateSumOfAllParts() {
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
  std::unordered_map<std::string, GearSymbols> gearSymbolsMap = {};
  int sumOfParts = 0;
};
