#include "Bus.h"

namespace Events {

HandlerList* Bus::GetList(std::type_index type) {
  auto it = subscribers.find(type);
  if (it == subscribers.end()) {
    // Create a new list of subscribers if one does not already exist
    auto list = std::make_unique<HandlerList>();
    it = subscribers.insert(std::make_pair(type, std::move(list))).first;
  }
  return it->second.get();
}

void Bus::AddItem(std::type_index type, IEventCallback* subscriber) {
  auto list = GetList(type);
  list->push_back(subscriber);
}

}  // namespace Events
