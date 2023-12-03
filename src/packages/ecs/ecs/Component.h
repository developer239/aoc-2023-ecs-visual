#pragma once

#include "IComponent.h"

namespace ECS {

template <typename TComponent>
class Component : public IComponent {
 public:
  static int GetId() {
    static auto id = nextId++;
    return id;
  }
};

}  // namespace ECS
