#include "./Bus.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class CustomEvent : public Events::EventBase {
 public:
  std::string message;
  int value;

  explicit CustomEvent(std::string message, int value)
      : message(std::move(message)), value(value) {}
};

class EventReceiver {
 public:
  MOCK_METHOD1(OnCustomEvent, void(CustomEvent&));
};

class EventTest : public ::testing::Test {
 protected:
  EventReceiver receiver;

  void SetUp() override {
    Events::Bus::Instance().SubscribeToEvent<CustomEvent>(&receiver, &EventReceiver::OnCustomEvent);
  }
};

TEST_F(EventTest, Subscription) {
  EXPECT_CALL(receiver, OnCustomEvent(::testing::_))
      .WillOnce([](CustomEvent& event) {
        EXPECT_EQ(event.message, "Hello, world!");
        EXPECT_EQ(event.value, 42);
      });

  Events::Bus::Instance().EmitEvent<CustomEvent>("Hello, world!", 42);
}

TEST_F(EventTest, Unsubscription) {
  Events::Bus::Instance().UnsubscribeFromEvent<CustomEvent>(
      &receiver,
      &EventReceiver::OnCustomEvent
  );

  Events::Bus::Instance().EmitEvent<CustomEvent>("Hello, again!", 41);

  EXPECT_CALL(receiver, OnCustomEvent(::testing::_)).Times(0);
}
