#pragma once

namespace ECS {

class IPool {
 public:
  virtual ~IPool() = default;

  virtual void RemoveEntityFromPool(int entityId) = 0;
};

}  // namespace ECS
