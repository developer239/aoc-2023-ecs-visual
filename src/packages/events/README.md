# Automation Engine - Events

Create and emit events. Event bus will automatically call registered event handlers when
an event is emitted.

## Example

```cpp
class CustomEvent : public Events::EventBase {
 public:
  std::string message;
  int value;

  explicit CustomEvent(std::string message, int value)
      : message(std::Move(message)), value(value) {}
};

class EventReceiver {
 public:
  void OnCustomEvent(CustomEvent& event) {
    std::cout << "Received event: " << event.message << " " << event.value << std::endl;
  }
};

main() {
  EventReceiver receiver;
  Events::Bus bus;
  
  bus.SubscribeToEvent<CustomEvent>(&receiver, &EventReceiver::OnCustomEvent);

  bus.EmitEvent<CustomEvent>("Hello, world!", 42);
}
```
