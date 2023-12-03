#pragma once

#include <utility>

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "core/IStrategy.h"
#include "core/Renderer.h"
#include "core/Window.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace CoreImGui {

class Strategy : public Core::IStrategy {
 public:
  ~Strategy();

  void Init(Core::Window& window, Core::Renderer& renderer) override;

  void HandleEvent(SDL_Event& event) override;

  void OnBeforeRender(Core::Window& window, Core::Renderer& renderer) override;

  void OnAfterRender(Core::Window& window, Core::Renderer& renderer) override;

  void OnRender(Core::Window& window, Core::Renderer& renderer) override;

  void OnUpdate(Core::Window& window, Core::Renderer& renderer, double deltaTime) override;
};

}  // namespace CoreImGui
