#pragma once

#include <functional>

namespace Events {

class EventBase {
 public:
  EventBase() = default;
};

class IEventCallback {
 private:
  virtual void Call(EventBase& event) = 0;

 public:
  virtual ~IEventCallback() = default;

  void Execute(EventBase& event) { Call(event); }
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
 public:
  TOwner* ownerInstance;

  typedef void (TOwner::*CallbackFunction)(TEvent&);
  CallbackFunction callbackFunction;

 private:
  void Call(EventBase& e) override {
    std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
  }

 public:
  EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
    this->ownerInstance = ownerInstance;
    this->callbackFunction = callbackFunction;
  }
};

}  // namespace Events
