#pragma once

#include <fstream>
#include <string>
#include <vector>

bool isSymbol(char ch) { return !std::isdigit(ch) && ch != '.'; }

struct Part {
  int row = -1;
  int column = -1;
  std::string number;
};

struct GearSymbols {
  int row = -1;
  int column = -1;
  std::string symbol;

  int adjacentCount = 0;
  std::vector<std::string> adjacentNumbers = {};
};

struct PartsAndSymbolsResult {
  std::vector<Part> parts = {};
  std::vector<GearSymbols> symbols = {};
};

PartsAndSymbolsResult FindPartsAndSymbols(const std::vector<std::string>& lines
) {
  std::vector<Part> parts = {};
  std::vector<GearSymbols> symbols = {};

  for (int row = 0; row < lines.size(); row++) {
    auto line = lines[row];
    std::string partNumber = "";
    int rowStart = 0;
    int columnStart = 0;

    for (int col = 0; col < line.length(); col++) {
      auto letter = line.at(col);

      if (std::isdigit(letter)) {
        if (partNumber.length() == 0) {
          rowStart = row;
          columnStart = col;
        }

        partNumber.push_back(letter);
        continue;
      }

      if (isSymbol(letter)) {
        symbols.push_back({
            .row = row,
            .column = col,
            .symbol = std::string(1, letter),
        });
      }

      // add number and reset START
      if (partNumber.length()) {
        parts.push_back({rowStart, columnStart, partNumber});

        partNumber = "";
      }
      // END
    }

    // add number and reset START
    if (partNumber.length()) {
      parts.push_back({rowStart, columnStart, partNumber});

      // technically you don't have to reset you only want to add what is left
      // in the memory
      partNumber = "";
    }
    // END
  }

  return {
      .parts = parts,
      .symbols = symbols,
  };
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
