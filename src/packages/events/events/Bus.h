#pragma once

#include <list>
#include <map>
#include <typeindex>

#include "./Event.h"

namespace Events {

typedef std::list<IEventCallback*> HandlerList;

class Bus {
 private:
  std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

  HandlerList* GetList(std::type_index type);

  void AddItem(std::type_index type, IEventCallback* subscriber);

  Bus() = default;

 public:
  Bus(const Bus&) = delete;
  Bus& operator=(const Bus&) = delete;

  static Bus& Instance() {
    static Bus instance;
    return instance;
  }

  template <typename TOwner, typename TEvent>
  IEventCallback* MakeEventCallback(
      TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)
  ) {
    return new EventCallback<TOwner, TEvent>(ownerInstance, callbackFunction);
  }

  template <typename TEvent, typename TOwner>
  void SubscribeToEvent(
      TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)
  ) {
    AddItem(typeid(TEvent), MakeEventCallback(ownerInstance, callbackFunction));
  }

  template <typename TEvent, typename TOwner>
  void UnsubscribeFromEvent(
      TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)
  ) {
    auto list = GetList(typeid(TEvent));
    auto it =
        std::find_if(list->begin(), list->end(), [&](IEventCallback* callback) {
          auto eventCallback =
              dynamic_cast<EventCallback<TOwner, TEvent>*>(callback);
          return eventCallback &&
                 eventCallback->ownerInstance == ownerInstance &&
                 eventCallback->callbackFunction == callbackFunction;
        });
    if (it != list->end()) {
      list->erase(it);
    }
  }

  template <typename TEvent, typename... TArgs>
  void EmitEvent(TArgs&&... args) {
    auto handlers = GetList(typeid(TEvent));

    if (handlers) {
      for (auto& it : *handlers) {
        auto handler = it;

        TEvent event(std::forward<TArgs>(args)...);
        handler->Execute(event);
      }
    }
  }
};

}  // namespace Events
