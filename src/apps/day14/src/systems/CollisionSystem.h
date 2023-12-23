#pragma once

#include "ecs/System.h"
#include "events/Bus.h"

#include "../components/BoxColliderComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../events/CollisionEvent.h"

#include <functional>  // For std::reference_wrapper
#include <tuple>
#include <vector>

class CollisionSystem : public ECS::System {
 public:
  CollisionSystem() {
    RequireComponent<RigidBodyComponent>();
    RequireComponent<BoxColliderComponent>();
  }

  void PreCalculatePairs() {
    auto entities = GetSystemEntities();
    collisionPairs.clear();

    for (auto i = entities.begin(); i != entities.end(); ++i) {
      auto& rigidBodyA =
          ECS::Registry::Instance().GetComponent<RigidBodyComponent>(*i);
      auto& colliderA =
          ECS::Registry::Instance().GetComponent<BoxColliderComponent>(*i);

      for (auto j = std::next(i); j != entities.end(); ++j) {
        auto& rigidBodyB =
            ECS::Registry::Instance().GetComponent<RigidBodyComponent>(*j);
        auto& colliderB =
            ECS::Registry::Instance().GetComponent<BoxColliderComponent>(*j);

        collisionPairs.emplace_back(
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

  void Update() {
    if (collisionPairs.empty()) {
      PreCalculatePairs();
    }

    for (const auto& [entityA, rigidBodyARef, colliderARef, entityB, rigidBodyBRef, colliderBRef] :
         collisionPairs) {
      auto& rigidBodyA = rigidBodyARef.get();
      auto& colliderA = colliderARef.get();
      auto& rigidBodyB = rigidBodyBRef.get();
      auto& colliderB = colliderBRef.get();

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
        Events::Bus::Instance().EmitEvent<CollisionEvent>(entityA, entityB);
        rigidBodyA.velocity.x = 0;
        rigidBodyA.velocity.y = 0;
        rigidBodyB.velocity.x = 0;
        rigidBodyB.velocity.y = 0;
      }
    }
  }

 private:
  struct RigidBodyAndColliderRef {
    ECS::Entity entity;
    std::reference_wrapper<RigidBodyComponent> rigidBody;
    std::reference_wrapper<BoxColliderComponent> collider;
  };

  std::vector<std::tuple<
      ECS::Entity, std::reference_wrapper<RigidBodyComponent>,
      std::reference_wrapper<BoxColliderComponent>, ECS::Entity,
      std::reference_wrapper<RigidBodyComponent>,
      std::reference_wrapper<BoxColliderComponent>>>
      collisionPairs;

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
