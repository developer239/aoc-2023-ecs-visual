#pragma once

#include "events/Event.h"
#include "ecs/Entity.h"

class CollisionEvent : public Events::EventBase {
 public:
  ECS::Entity a;
  ECS::Entity b;

  CollisionEvent(ECS::Entity a, ECS::Entity b) : a(a), b(b) {
  }
};
