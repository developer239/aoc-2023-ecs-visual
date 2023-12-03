#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "ecs/System.h"
#include "../components/RigidBodyComponent.h"
#include "../components/TextComponent.h"

class RenderTextSystem : public ECS::System {
 public:
  RenderTextSystem() {
    RequireComponent<TextComponent>();
    RequireComponent<RigidBodyComponent>();
  }

  void Render(Core::Renderer& renderer) {
    for (auto entity : GetSystemEntities()) {
      const auto& textLabel = ECS::Registry::Instance().GetComponent<TextComponent>(entity);
      const auto& rigidBody = ECS::Registry::Instance().GetComponent<RigidBodyComponent>(entity);

      // Use the font from the asset store
      TTF_Font* font = Core::AssetStore::Instance().GetFont(textLabel.fontId).get();
      if (!font) {
        // Handle error: Font not found
        continue;
      }

      SDL_Surface* surface = TTF_RenderText_Blended(font, textLabel.text.c_str(), textLabel.color);
      if (!surface) {
        // Handle error: Unable to create surface
        continue;
      }

      SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.Get().get(), surface);
      SDL_FreeSurface(surface);
      if (!texture) {
        // Handle error: Unable to create texture
        continue;
      }

      int textWidth = 0, textHeight = 0;
      SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);

      // Center the text within the rigid body
      int xPosition = static_cast<int>(rigidBody.position.x + (rigidBody.width - textWidth) / 2);
      int yPosition = static_cast<int>(rigidBody.position.y + (rigidBody.height - textHeight) / 2);

      SDL_Rect dstRect = { xPosition, yPosition, textWidth, textHeight };

      SDL_RenderCopy(renderer.Get().get(), texture, nullptr, &dstRect);

      SDL_DestroyTexture(texture);
    }
  }
};
