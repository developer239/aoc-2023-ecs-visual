#pragma once

#include "IPool.h"

namespace ECS {

template <typename TItem>
class Pool : public IPool {
 private:
  std::vector<TItem> data;
  int size;

  std::unordered_map<int, int> entityIdToIndex;
  std::unordered_map<int, int> indexToEntityId;

 public:
  explicit Pool(int capacity = 100) {
    size = 0;
    data.resize(capacity);
  }

  ~Pool() override = default;

  [[nodiscard]] bool IsEmpty() const { return size == 0; }

  [[nodiscard]] int GetSize() const { return size; }

  void Clear() {
    data.clear();
    entityIdToIndex.clear();
    indexToEntityId.clear();
    size = 0;
  }

  void Set(int entityId, TItem object) {
    if (entityIdToIndex.find(entityId) != entityIdToIndex.end()) {
      int index = entityIdToIndex[entityId];
      data[index] = object;
    } else {
      int index = size;
      entityIdToIndex.emplace(entityId, index);
      indexToEntityId.emplace(index, entityId);

      if (index >= data.capacity()) {
        data.resize(size * 2);
      }

      data[index] = object;
      size++;
    }
  }

  void Remove(int entityId) {
    int indexOfRemoved = entityIdToIndex[entityId];
    int indexOfLast = size - 1;
    data[indexOfRemoved] = data[indexOfLast];

    int entityIdOfLastElement = indexToEntityId[indexOfLast];
    entityIdToIndex[entityIdOfLastElement] = indexOfRemoved;
    indexToEntityId[indexOfRemoved] = entityIdOfLastElement;

    entityIdToIndex.erase(entityId);
    indexToEntityId.erase(indexOfLast);

    size--;
  }

  void RemoveEntityFromPool(int entityId) override {
    if (entityIdToIndex.find(entityId) != entityIdToIndex.end()) {
      Remove(entityId);
    }
  }

  TItem& Get(int entityId) {
    int index = entityIdToIndex[entityId];
    return static_cast<TItem&>(data[index]);
  }

  TItem& operator[](unsigned int index) { return data[index]; }
};

}  // namespace ECS
