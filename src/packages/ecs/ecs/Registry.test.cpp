#include "Registry.h"
#include "Component.h"
#include "Entity.h"
#include "System.h"
#include "gtest/gtest.h"

struct PositionComponent : public ECS::Component<PositionComponent> {
  float x = 0;
  float y = 0;
};

class PositionSystem : public ECS::System {
 public:
  PositionSystem() { RequireComponent<PositionComponent>(); }

  void Update() {
    for (auto entity : GetSystemEntities()) {
      auto& component = ECS::Registry::Instance().GetComponent<PositionComponent>(entity);
      component.x += 1;
      component.y += 1;
    }
  }
};

class ECSIntegrationTest : public testing::Test {
 protected:
  ECSIntegrationTest(): entity(ECS::Registry::Instance().CreateEntity()) {
    entity = ECS::Registry::Instance().CreateEntity();
    ECS::Registry::Instance().AddComponentToEntity<PositionComponent>(entity);
    ECS::Registry::Instance().AddSystem<PositionSystem>();
  }

  ECS::Entity entity;
};

TEST_F(ECSIntegrationTest, TestPositionSystemUpdatesPosition) {
  auto& position = ECS::Registry::Instance().GetComponent<PositionComponent>(entity);

  EXPECT_FLOAT_EQ(position.x, 0);
  EXPECT_FLOAT_EQ(position.y, 0);

  ECS::Registry::Instance().Update();
  ECS::Registry::Instance().GetSystem<PositionSystem>().Update();

  EXPECT_FLOAT_EQ(position.x, 1);
  EXPECT_FLOAT_EQ(position.y, 1);
}
