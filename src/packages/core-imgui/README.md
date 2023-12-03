# Automation Engine - Core ImGui

Core app loop strategy implementation to provide support for ImGui on top of SDL.

## Example

```cpp
int main() {
  CoreImGui::Strategy gui;

  auto loop = Core::Loop({
    &gui
  });
  loop.Run();
}
```
