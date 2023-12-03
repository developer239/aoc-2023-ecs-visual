#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <string>
#include <memory>

#include "./Renderer.h"

namespace Core {

class AssetStore {
 private:
  std::map<std::string, std::shared_ptr<SDL_Texture>> textures;
  std::map<std::string, std::shared_ptr<TTF_Font>> fonts;

  AssetStore() = default;

 public:
  AssetStore(const AssetStore&) = delete;
  AssetStore& operator=(const AssetStore&) = delete;

  static AssetStore& Instance() {
    static AssetStore instance;
    return instance;
  }

  void ClearAssets();

  void AddTexture(
      Renderer renderer, const std::string& assetId, const std::string& filePath
  );

  std::shared_ptr<SDL_Texture> GetTexture(const std::string& assetId);

  void AddFont(
      const std::string& assetId, const std::string& filePath, int fontSize
  );

  std::shared_ptr<TTF_Font> GetFont(const std::string& assetId);
};

}  // namespace Core
