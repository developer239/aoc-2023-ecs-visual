# Automation Engine - Entity Component System

Optional ECS utility package that makes it easy to create and manage entities and components and structure the app
logic.

## Example

```cpp
struct PositionComponent : public ECS::Component<PositionComponent> {
  float x = 0;
  float y = 0;
};

class PositionSystem : public ECS::System {
 public:
  PositionSystem() { RequireComponent<PositionComponent>(); }

  void RenderColorBuffer() {
    for (auto entity : GetSystemEntities()) {
      auto& component = ECS::Registry::Instance().GetComponent<PositionComponent>(entity);
      component.x += 1;
      component.y += 1;
    }
  }
};

main() {
  ECS::Entity entity;
  PositionSystem positionSystem;
  
  ECS::Registry::Instance().AddComponentToEntity<PositionComponent>(entity);
  ECS::Registry::Instance().AddSystem<PositionSystem>();
    
  auto& position = ECS::Registry::Instance().GetComponent<PositionComponent>(entity);

  EXPECT_FLOAT_EQ(position.x, 0);
  EXPECT_FLOAT_EQ(position.y, 0);

  ECS::Registry::Instance().RenderColorBuffer();
  ECS::Registry::Instance().GetSystem<PositionSystem>().RenderColorBuffer();

  EXPECT_FLOAT_EQ(position.x, 1);
  EXPECT_FLOAT_EQ(position.y, 1);  
}

```
