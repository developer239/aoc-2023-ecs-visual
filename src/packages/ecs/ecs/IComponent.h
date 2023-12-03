#pragma once

namespace ECS {

struct IComponent {
 protected:
  static int nextId;
};

}  // namespace ECS