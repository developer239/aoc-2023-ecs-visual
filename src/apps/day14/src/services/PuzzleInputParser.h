#pragma once

#include <fstream>
#include <string>
#include <vector>

enum TileType { ROUNDED_ROCK, CUBE_SHAPED_ROCK, EMPTY_SPACE };

struct Tile {
 public:
  TileType type;

  int row;
  int col;

  int initialRow;
  int initialCol;

  static TileType getTypeFromChar(char letter) {
    switch (letter) {
      case '#':
        return CUBE_SHAPED_ROCK;
      case 'O':
        return ROUNDED_ROCK;
      case '.':
        return EMPTY_SPACE;
      default:
        throw std::runtime_error("Unknown tile type");
    }
  }
};

struct Platform {
  std::vector<std::vector<Tile>> tiles;

  std::vector<Tile> GetTiles(TileType type) const {
    std::vector<Tile> result = {};

    for (const auto& row : tiles) {
      for (const auto& tile : row) {
        if (tile.type == type) {
          result.push_back(tile);
        }
      }
    }

    return result;
  }
};

Platform BuildPlatformFromInput(const std::vector<std::string>& lines) {
  std::vector<std::vector<Tile>> tiles = {};

  for (int row = 0; row < lines.size(); row++) {
    auto line = lines[row];

    for (int col = 0; col < line.size(); col++) {
      auto symbolType = Tile::getTypeFromChar(line[col]);

      // add row col vector if not set
      if (tiles.size() <= row) {
        tiles.push_back({});
      }

      tiles[row].push_back(Tile{symbolType, row, col, row, col});
    }
  }

  return {tiles};
}

std::vector<std::string> ParseInput(const std::string& filePath) {
  std::vector<std::string> inputData;
  std::ifstream inFile(filePath);
  std::string line;

  while (std::getline(inFile, line)) {
    inputData.push_back(line);
  }

  return inputData;
}
