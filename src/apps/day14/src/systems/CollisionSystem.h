#pragma once

#include "ecs/System.h"
#include "events/Bus.h"

#include "../components/BoxColliderComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../events/CollisionEvent.h"

#include <functional>
#include <tuple>
#include <unordered_map>
#include <vector>

class CollisionSystem : public ECS::System {
 public:
  float scale;

  CollisionSystem(float scale) : scale(scale) {
    RequireComponent<RigidBodyComponent>();
    RequireComponent<BoxColliderComponent>();
  }

  void PreCalculatePairs() {
    auto entities = GetSystemEntities();
    collisionPairsByColumn.clear();

    for (auto i = entities.begin(); i != entities.end(); ++i) {
      auto& rigidBodyA =
          ECS::Registry::Instance().GetComponent<RigidBodyComponent>(*i);
      auto& colliderA =
          ECS::Registry::Instance().GetComponent<BoxColliderComponent>(*i);
      int columnA = GetColumn(rigidBodyA);

      for (auto j = std::next(i); j != entities.end(); ++j) {
        auto& rigidBodyB =
            ECS::Registry::Instance().GetComponent<RigidBodyComponent>(*j);
        auto& colliderB =
            ECS::Registry::Instance().GetComponent<BoxColliderComponent>(*j);
        int columnB = GetColumn(rigidBodyB);

        if (columnA == columnB) {
          collisionPairsByColumn[columnA].emplace_back(
              *i,
              std::ref(rigidBodyA),
              std::ref(colliderA),
              *j,
              std::ref(rigidBodyB),
              std::ref(colliderB)
          );
        }
      }
    }
  }

  void MoveEntities(int column) {
    auto entities = GetSystemEntities();

    for (auto entity : entities) {
      auto& rigidBody =
          ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);
      int entityColumn = GetColumn(rigidBody);

      if (entityColumn == column) {
        rigidBody.position.x += rigidBody.velocity.x;
        rigidBody.position.y += rigidBody.velocity.y;
      }
    }
  }

  void Update(int column) {
    if (collisionPairsByColumn.empty()) {
      PreCalculatePairs();
    }

    MoveEntities(column);

    if (collisionPairsByColumn.find(column) != collisionPairsByColumn.end()) {
      for (const auto& pair : collisionPairsByColumn[column]) {
        auto& rigidBodyA = std::get<1>(pair).get();
        auto& colliderA = std::get<2>(pair).get();
        auto& rigidBodyB = std::get<4>(pair).get();
        auto& colliderB = std::get<5>(pair).get();

        PreventBoundaryEscape(rigidBodyA);
        PreventBoundaryEscape(rigidBodyB);

        bool collisionHappened = CheckAABBCollision(
            rigidBodyA.position.x + colliderA.offset.x,
            rigidBodyA.position.y + colliderA.offset.y,
            colliderA.width,
            colliderA.height,
            rigidBodyB.position.x + colliderB.offset.x,
            rigidBodyB.position.y + colliderB.offset.y,
            colliderB.width,
            colliderB.height
        );

        if (collisionHappened) {
          Events::Bus::Instance().EmitEvent<CollisionEvent>(
              std::get<0>(pair),
              std::get<3>(pair)
          );
          rigidBodyA.velocity.x = 0;
          rigidBodyA.velocity.y = 0;
          rigidBodyB.velocity.x = 0;
          rigidBodyB.velocity.y = 0;
        }
      }
    }
  }

 private:
  std::unordered_map<
      int, std::vector<std::tuple<
               ECS::Entity, std::reference_wrapper<RigidBodyComponent>,
               std::reference_wrapper<BoxColliderComponent>, ECS::Entity,
               std::reference_wrapper<RigidBodyComponent>,
               std::reference_wrapper<BoxColliderComponent>>>>
      collisionPairsByColumn;

  int GetColumn(const RigidBodyComponent& rigidBody) {
    return static_cast<int>(rigidBody.position.x / scale);
  }

  void PreventBoundaryEscape(RigidBodyComponent& rigidBody) {
    if (rigidBody.position.x < 0 || rigidBody.position.y < 0) {
      rigidBody.velocity.x = 0;
      rigidBody.velocity.y = 0;
    }
  }

  bool CheckAABBCollision(
      double aX, double aY, double aW, double aH, double bX, double bY,
      double bW, double bH
  ) {
    return (aX < bX + bW && aX + aW > bX && aY < bY + bH && aY + aH > bY);
  }
};
