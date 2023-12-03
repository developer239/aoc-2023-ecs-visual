#pragma once

#include <string>

namespace ECS {

class Entity {
 private:
  int id;

 public:
  explicit Entity(int id) { this->id = id; };

  [[nodiscard]] int GetId() const;

  std::string ToString() const;

  Entity& operator=(const Entity& other) = default;

  bool operator==(const Entity& other) const { return id == other.id; }

  bool operator!=(const Entity& other) const { return id != other.id; }

  bool operator>(const Entity& other) const { return id > other.id; }

  bool operator<(const Entity& other) const { return id < other.id; }
};

}  // namespace ECS
