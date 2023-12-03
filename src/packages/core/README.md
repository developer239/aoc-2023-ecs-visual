# Automation Engine - Core

SDL game loop implementation. It exposes IStrategy interface to the user, which is used to implement the app logic.

## Example

```cpp
class CustomStrategy : public Core::IStrategy {
 public:
  void Init(Core::Window& window, Core::Renderer& renderer) override {
    std::cout << "Init" << std::endl;
  }

  void HandleEvent(SDL_Event& event) override {
    std::cout << "HandleEvent" << std::endl;
  }

  void OnRender(Window& window, Renderer& renderer) override {
    std::cout << "OnRender" << std::endl;
  }
};

int main() {
  CustomStrategy strategy;

  auto loop = Core::Loop({&strategy});
  loop.Run();
}
```
