#include "Entity.h"
#include "Registry.h"

namespace ECS {

int Entity::GetId() const { return id; }

std::string Entity::ToString() const { return "Entity " + std::to_string(id); }

}  // namespace ECS
