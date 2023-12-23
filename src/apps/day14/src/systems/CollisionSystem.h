#pragma once

#include "ecs/System.h"
#include "events/Bus.h"

#include "../components/BoxColliderComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../events/CollisionEvent.h"

class CollisionSystem : public ECS::System {
 public:
  CollisionSystem() {
    RequireComponent<RigidBodyComponent>();
    RequireComponent<BoxColliderComponent>();
  }

  void Update() {
    auto entities = GetSystemEntities();

    for (auto i = entities.begin(); i != entities.end(); ++i) {
      ECS::Entity a = *i;
      auto& aRigidBody = ECS::Registry::Instance().GetComponent<RigidBodyComponent>(a);
      auto& aCollider = ECS::Registry::Instance().GetComponent<BoxColliderComponent>(a);

      // Prevent entities from getting outside of the screen boundaries
      if (aRigidBody.position.x < 0 || aRigidBody.position.y < 0) {
        aRigidBody.velocity.x = 0;
        aRigidBody.velocity.y = 0;
      }

      for (auto j = std::next(i); j != entities.end(); ++j) {
        ECS::Entity b = *j;
        auto& bRigidBody = ECS::Registry::Instance().GetComponent<RigidBodyComponent>(b);
        auto& bCollider = ECS::Registry::Instance().GetComponent<BoxColliderComponent>(b);

        // Prevent entities from getting outside of the screen boundaries
        if (bRigidBody.position.x < 0 || bRigidBody.position.y < 0) {
          bRigidBody.velocity.x = 0;
          bRigidBody.velocity.y = 0;
        }

        // Only check for collisions if exactly one entity has non-zero vertical velocity
        if (!((aRigidBody.velocity.y != 0) ^ (bRigidBody.velocity.y != 0))) {
          continue;
        }

        bool collisionHappened = CheckAABBCollision(
            aRigidBody.position.x + aCollider.offset.x,
            aRigidBody.position.y + aCollider.offset.y,
            aCollider.width,
            aCollider.height,
            bRigidBody.position.x + bCollider.offset.x,
            bRigidBody.position.y + bCollider.offset.y,
            bCollider.width,
            bCollider.height
        );

        if (collisionHappened) {
          Events::Bus::Instance().EmitEvent<CollisionEvent>(a, b);

          aRigidBody.velocity.x = 0;
          aRigidBody.velocity.y = 0;
          bRigidBody.velocity.x = 0;
          bRigidBody.velocity.y = 0;
        }
      }
    }
  }

  bool CheckAABBCollision(
      double aX, double aY, double aW, double aH, double bX, double bY,
      double bW, double bH
  ) {
    return (aX < bX + bW && aX + aW > bX && aY < bY + bH && aY + aH > bY);
  }
};
