#pragma once

#include <bitset>
#include <deque>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "Pool.h"
#include "Signature.h"
#include "System.h"

namespace ECS {

class Registry {
 private:
  Registry() = default;

  int numEntities = 0;

  std::vector<std::shared_ptr<IPool>> componentPools;

  std::vector<Signature> entityComponentSignatures;

  std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

  std::set<Entity> entitiesToBeAdded;
  std::set<Entity> entitiesToBeKilled;

  std::unordered_map<std::string, Entity> entityPerTag;
  std::unordered_map<int, std::string> tagPerEntity;

  std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
  std::unordered_map<int, std::set<std::string>> groupsPerEntity;

  std::deque<int> freeIds;

 public:
  Registry(const Registry&) = delete;
  Registry& operator=(const Registry&) = delete;

  static Registry& Instance() {
    static Registry instance;
    return instance;
  }

  Entity CreateEntity() {
    int entityId;

    if (freeIds.empty()) {
      entityId = numEntities++;
      if (entityId >= entityComponentSignatures.size()) {
        entityComponentSignatures.resize(entityId + 1);
      }
    } else {
      entityId = freeIds.front();
      freeIds.pop_front();
    }

    Entity entity(entityId);
    entitiesToBeAdded.insert(entity);

    return entity;
  }

  void KillEntity(Entity entity) { entitiesToBeKilled.insert(entity); }

  void AddEntityToSystems(Entity entity) {
    const auto entityId = entity.GetId();

    const auto& entityComponentSignature = entityComponentSignatures[entityId];

    for (auto& system : systems) {
      const auto& systemComponentSignature =
          system.second->GetComponentSignature();

      bool isInterested = (entityComponentSignature & systemComponentSignature
                          ) == systemComponentSignature;

      if (isInterested) {
        system.second->AddEntityToSystem(entity);
      }
    }
  }

  void RemoveEntityFromSystems(Entity entity) {
    for (const auto& system : systems) {
      system.second->RemoveEntityFromSystem(entity);
    }
  }

  void RemoveAllEntitiesFromSystems() {
    for (const auto& system : systems) {
      system.second->RemoveAllEntitiesFromSystem();
    }
  }

  void TagEntity(Entity entity, const std::string& tag) {
    entityPerTag.emplace(tag, entity);
    tagPerEntity.emplace(entity.GetId(), tag);
  }

  [[nodiscard]] std::string GetEntityTag(Entity entity) const {
    if (!tagPerEntity.empty() && entity.GetId() < tagPerEntity.size()) {
      return tagPerEntity.at(entity.GetId());
    }

    return "no-tag";
  }

  bool DoesEntityHaveTag(Entity entity, std::string tag) {
    return GetEntityTag(entity) == tag;
  }

  std::vector<std::string> GetEntityGroups(Entity entity) const {
    std::vector<std::string> groups = {};

    try {
      for (const auto& group : groupsPerEntity.at(entity.GetId())) {
        groups.push_back(group);
      }

      return groups;
    } catch (const std::out_of_range& e) {
      return groups;
    }
  }

  [[nodiscard]] Entity GetEntityByTag(const std::string& tag) const {
    try {
      return entityPerTag.at(tag);
    } catch (const std::out_of_range& e) {
      throw std::runtime_error("No entity with tag " + tag);
    }
  }

  void RemoveEntityTag(Entity entity) {
    auto taggedEntity = tagPerEntity.find(entity.GetId());
    if (taggedEntity != tagPerEntity.end()) {
      auto tag = taggedEntity->second;
      entityPerTag.erase(tag);
      tagPerEntity.erase(taggedEntity);
    }
  }

  void GroupEntity(Entity entity, const std::string& group) {
    entitiesPerGroup.emplace(group, std::set<Entity>());
    entitiesPerGroup[group].emplace(entity);

    groupsPerEntity.emplace(entity.GetId(), std::set<std::string>());
    groupsPerEntity[entity.GetId()].emplace(group);
  }

  std::vector<Entity> GetEntitiesByGroup(const std::string& group) const {
    try {
      auto& setOfEntities = entitiesPerGroup.at(group);
      return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
    } catch (const std::out_of_range& e) {
      return std::vector<Entity>();
    }
  }

  void RemoveEntityGroup(Entity entity, std::string group) {
    auto& entitySet = entitiesPerGroup[group];
    entitySet.erase(entity);

    auto& groupSet = groupsPerEntity[entity.GetId()];
    groupSet.erase(group);

    if (groupSet.empty()) {
      groupsPerEntity.erase(entity.GetId());
    }
  }

  void RemoveEntityGroups(Entity entity) {
    auto& groupSet = groupsPerEntity[entity.GetId()];

    for (const auto& group : groupSet) {
      auto& entitySet = entitiesPerGroup[group];
      entitySet.erase(entity);
    }

    groupsPerEntity.erase(entity.GetId());
  }

  void Update() {
    for (auto entity : entitiesToBeAdded) {
      AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    for (auto entity : entitiesToBeKilled) {
      RemoveEntityFromSystems(entity);
      entityComponentSignatures[entity.GetId()].reset();

      for (auto pool : componentPools) {
        if (pool) {
          pool->RemoveEntityFromPool(entity.GetId());
        }
      }

      freeIds.push_back(entity.GetId());

      RemoveEntityTag(entity);
      RemoveEntityGroups(entity);
    }
    entitiesToBeKilled.clear();
  }

  template <typename TSystem, typename... TArgs>
  void AddSystem(TArgs&&... args) {
    std::shared_ptr<TSystem> newSystem =
        std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
  }

  template <typename TSystem>
  void RemoveSystem() {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
  }

  template <typename TSystem>
  bool HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
  }

  template <typename TSystem>
  TSystem& GetSystem() const {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
  }

  template <typename TComponent>
  std::vector<Entity> GetEntitiesWithComponent() const {
    std::vector<Entity> entitiesWithComponent;

    const auto componentId = Component<TComponent>::GetId();
    for (size_t entityId = 0; entityId < entityComponentSignatures.size();
         ++entityId) {
      if (entityComponentSignatures[entityId].test(componentId)) {
        entitiesWithComponent.emplace_back(Entity(static_cast<int>(entityId)));
      }
    }

    return entitiesWithComponent;
  }

  template <typename TComponent, typename... TArgs>
  void AddComponent(Entity entity, TArgs&&... args) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    if (componentId >= componentPools.size()) {
      componentPools.resize(componentId + 1, nullptr);
    }

    if (!componentPools[componentId]) {
      std::shared_ptr<Pool<TComponent>> newComponentPool(new Pool<TComponent>()
      );
      componentPools[componentId] = newComponentPool;
    }

    std::shared_ptr<Pool<TComponent>> componentPool =
        std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId, newComponent);

    entityComponentSignatures[entityId].set(componentId);
  }

  template <typename TComponent>
  void RemoveComponent(Entity entity) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    std::shared_ptr<Pool<TComponent>> componentPool =
        std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    componentPool->Remove(entityId);

    entityComponentSignatures[entityId].set(componentId, false);
  }

  template <typename TComponent>
  bool HasComponent(Entity entity) const {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    return entityComponentSignatures[entityId].test(componentId);
  }

  template <typename TComponent>
  TComponent& GetComponent(Entity entity) const {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    auto componentPool =
        std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
    return componentPool->Get(entityId);
  }

  template <typename TComponent, typename... TArgs>
  void AddComponentToEntity(Entity& entity, TArgs&&... args) {
    AddComponent<TComponent>(entity, std::forward<TArgs>(args)...);
  }

  template <typename TComponent>
  void RemoveComponentFromEntity(Entity& entity) {
    RemoveComponent<TComponent>(entity);
  }

  template <typename TComponent>
  bool HasEntityComponent(Entity& entity) const {
    return HasComponent<TComponent>(entity);
  }

  template <typename TComponent>
  TComponent& GetEntityComponent(Entity& entity) const {
    return GetComponent<TComponent>(entity);
  }
};

}  // namespace ECS
