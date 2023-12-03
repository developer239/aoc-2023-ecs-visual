#include <SDL_image.h>
#include <filesystem>

#include "logger/Logger.h"

#include "./AssetStore.h"

namespace Core {

void AssetStore::ClearAssets() {
  textures.clear();
  fonts.clear();
}

void AssetStore::AddTexture(
    Renderer renderer, const std::string& assetId, const std::string& filePath
) {
  SDL_Surface* surface = IMG_Load(filePath.c_str());
  std::shared_ptr<SDL_Texture> texture(
      SDL_CreateTextureFromSurface(renderer.Get().get(), surface),
      SDL_DestroyTexture
  );
  SDL_FreeSurface(surface);

  textures.emplace(assetId, texture);
}

std::shared_ptr<SDL_Texture> AssetStore::GetTexture(const std::string& assetId) {
  return textures[assetId];
}

void AssetStore::AddFont(
    const std::string& assetId, const std::string& filePath, int fontSize
) {
  auto status = std::filesystem::exists(filePath);
  if (!status) {
    Logger::Err("Couldn't load font " + assetId + " from " + filePath);
    return;
  }

  std::shared_ptr<TTF_Font> font(
      TTF_OpenFont(filePath.c_str(), fontSize),
      TTF_CloseFont
  );
  fonts.emplace(assetId, font);
}

std::shared_ptr<TTF_Font> AssetStore::GetFont(const std::string& assetId) {
  return fonts[assetId];
}

}  // namespace Core
