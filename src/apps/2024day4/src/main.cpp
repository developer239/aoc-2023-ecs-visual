#include <algorithm>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

// Define colors for each letter
const cv::Vec3b COLOR_X(0, 0, 255);    // Red
const cv::Vec3b COLOR_M(0, 255, 0);    // Green
const cv::Vec3b COLOR_A(255, 0, 0);    // Blue
const cv::Vec3b COLOR_S(255, 255, 0);  // Yellow
const cv::Vec3b COLOR_BG(0, 0, 0);     // Black background

struct Pattern {
  cv::Mat template_image;
  cv::Mat mask;
  std::string name;
  cv::Point offset;
  std::vector<std::pair<cv::Vec3b, cv::Point>> pattern_def;
};

std::vector<std::string> ParseInput(const std::string& filePath) {
  std::vector<std::string> inputData;
  std::ifstream inFile(filePath);
  std::string line;

  while (std::getline(inFile, line)) {
    if (!line.empty()) {
      inputData.push_back(line);
    }
  }
  return inputData;
}

cv::Mat createColoredGrid(const std::vector<std::string>& input) {
  int gridHeight = input.size();
  int gridWidth = input[0].size();
  cv::Mat colorGrid(gridHeight, gridWidth, CV_8UC3, COLOR_BG);

  for (int y = 0; y < gridHeight; y++) {
    for (int x = 0; x < gridWidth; x++) {
      cv::Vec3b color;
      switch (input[y][x]) {
        case 'X':
          color = COLOR_X;
          break;
        case 'M':
          color = COLOR_M;
          break;
        case 'A':
          color = COLOR_A;
          break;
        case 'S':
          color = COLOR_S;
          break;
        default:
          color = COLOR_BG;
          break;
      }
      colorGrid.at<cv::Vec3b>(y, x) = color;
    }
  }
  return colorGrid;
}

cv::Mat createPatternTemplate(
    const std::vector<std::pair<cv::Vec3b, cv::Point>>& pattern, cv::Mat& mask
) {
  int max_x = 0, max_y = 0;
  for (const auto& [color, point] : pattern) {
    max_x = std::max(max_x, point.x + 1);
    max_y = std::max(max_y, point.y + 1);
  }

  cv::Mat templ(max_y, max_x, CV_8UC3, COLOR_BG);
  mask = cv::Mat::zeros(max_y, max_x, CV_8UC1);

  for (const auto& [color, point] : pattern) {
    templ.at<cv::Vec3b>(point.y, point.x) = color;
    mask.at<uchar>(point.y, point.x) = 255;
  }

  return templ;
}

std::vector<Pattern> createPatterns() {
  std::vector<Pattern> patterns;

  std::vector<
      std::pair<std::vector<std::pair<cv::Vec3b, cv::Point>>, std::string>>
      patternDefs = {// XMAS patterns
                     {{{COLOR_X, {0, 0}},
                       {COLOR_M, {1, 1}},
                       {COLOR_A, {2, 2}},
                       {COLOR_S, {3, 3}}},
                      "diagonal-down-right"},
                     {{{COLOR_X, {0, 3}},
                       {COLOR_M, {1, 2}},
                       {COLOR_A, {2, 1}},
                       {COLOR_S, {3, 0}}},
                      "diagonal-up-right"},
                     {{{COLOR_X, {0, 0}},
                       {COLOR_M, {1, 0}},
                       {COLOR_A, {2, 0}},
                       {COLOR_S, {3, 0}}},
                      "horizontal-right"},
                     {{{COLOR_X, {0, 0}},
                       {COLOR_M, {0, 1}},
                       {COLOR_A, {0, 2}},
                       {COLOR_S, {0, 3}}},
                      "vertical-down"},
                     // SAMX patterns (reversed)
                     {{{COLOR_S, {0, 0}},
                       {COLOR_A, {1, 1}},
                       {COLOR_M, {2, 2}},
                       {COLOR_X, {3, 3}}},
                      "diagonal-down-right-rev"},
                     {{{COLOR_S, {0, 3}},
                       {COLOR_A, {1, 2}},
                       {COLOR_M, {2, 1}},
                       {COLOR_X, {3, 0}}},
                      "diagonal-up-right-rev"},
                     {{{COLOR_S, {0, 0}},
                       {COLOR_A, {1, 0}},
                       {COLOR_M, {2, 0}},
                       {COLOR_X, {3, 0}}},
                      "horizontal-right-rev"},
                     {{{COLOR_S, {0, 0}},
                       {COLOR_A, {0, 1}},
                       {COLOR_M, {0, 2}},
                       {COLOR_X, {0, 3}}},
                      "vertical-down-rev"}
      };

  for (const auto& [patternDef, name] : patternDefs) {
    cv::Mat mask;
    cv::Mat templ = createPatternTemplate(patternDef, mask);
    patterns.push_back({templ, mask, name, patternDef[0].second, patternDef});
  }

  return patterns;
}

bool verifyMatch(
    const cv::Mat& grid, const Pattern& pattern, const cv::Point& location
) {
  for (const auto& [color, offset] : pattern.pattern_def) {
    cv::Point pixelLoc(location.x + offset.x, location.y + offset.y);

    if (pixelLoc.x < 0 || pixelLoc.x >= grid.cols || pixelLoc.y < 0 ||
        pixelLoc.y >= grid.rows) {
      return false;
    }

    cv::Vec3b gridColor = grid.at<cv::Vec3b>(pixelLoc);
    if (gridColor != color) {
      return false;
    }
  }
  return true;
}

struct Match {
  cv::Point location;
  std::string patternType;
};

std::vector<Match> findMatches(
    const cv::Mat& grid, const std::vector<Pattern>& patterns,
    double threshold = 0.99
) {
  std::vector<Match> matches;
  std::set<std::string> uniqueMatches;

  for (const auto& pattern : patterns) {
    cv::Mat result;
    cv::matchTemplate(
        grid,
        pattern.template_image,
        result,
        cv::TM_CCOEFF_NORMED,
        pattern.mask
    );

    cv::Mat matches_mask;
    cv::threshold(result, matches_mask, threshold, 1.0, cv::THRESH_BINARY);

    std::vector<cv::Point> locations;
    cv::findNonZero(matches_mask, locations);

    for (const auto& loc : locations) {
      if (verifyMatch(grid, pattern, loc)) {
        std::string matchKey;
        for (const auto& [color, offset] : pattern.pattern_def) {
          cv::Point pixelLoc(loc.x + offset.x, loc.y + offset.y);
          matchKey += std::to_string(pixelLoc.x) + "," +
                      std::to_string(pixelLoc.y) + ";";
        }

        if (uniqueMatches.find(matchKey) == uniqueMatches.end()) {
          matches.push_back({loc, pattern.name});
          uniqueMatches.insert(matchKey);
        }
      }
    }
  }

  return matches;
}

cv::Mat scaleForVisualization(const cv::Mat& grid, int pixelSize) {
  cv::Mat
      scaled(grid.rows * pixelSize, grid.cols * pixelSize, CV_8UC3, COLOR_BG);

  for (int y = 0; y < grid.rows; y++) {
    for (int x = 0; x < grid.cols; x++) {
      cv::Rect rect(x * pixelSize, y * pixelSize, pixelSize, pixelSize);
      cv::rectangle(scaled, rect, grid.at<cv::Vec3b>(y, x), cv::FILLED);
    }
  }

  return scaled;
}

cv::Mat visualizeMatches(
    const cv::Mat& grid, const std::vector<Match>& matches, int pixelSize
) {
  cv::Mat scaled = scaleForVisualization(grid, pixelSize);

  for (const auto& match : matches) {
    cv::Point start(match.location.x * pixelSize, match.location.y * pixelSize);
    cv::Point end;
    cv::Scalar color(0, 0, 0);

    if (match.patternType.find("diagonal-down-right") != std::string::npos) {
      end = cv::Point(start.x + 4 * pixelSize, start.y + 4 * pixelSize);
    } else if (match.patternType.find("diagonal-up-right") !=
               std::string::npos) {
      end = cv::Point(start.x + 4 * pixelSize, start.y - 4 * pixelSize);
    } else if (match.patternType.find("horizontal") != std::string::npos) {
      end = cv::Point(start.x + 4 * pixelSize, start.y);
    } else if (match.patternType.find("vertical") != std::string::npos) {
      end = cv::Point(start.x, start.y + 4 * pixelSize);
    }

    cv::line(scaled, start, end, color, 2);
  }

  return scaled;
}

int main() {
  const std::string filePath = "assets/input-example-1.txt";
  const int VISUALIZATION_PIXEL_SIZE =
      30;  // This is now only for visualization

  auto input = ParseInput(filePath);
  cv::Mat colorGrid = createColoredGrid(input);
  auto patterns = createPatterns();

  auto matches = findMatches(colorGrid, patterns);
  std::cout << "Found " << matches.size() << " XMAS patterns" << std::endl;

  cv::Mat visualization =
      visualizeMatches(colorGrid, matches, VISUALIZATION_PIXEL_SIZE);
  cv::imwrite("matches.png", visualization);
  cv::imshow("XMAS Pattern Matches", visualization);
  cv::waitKey(0);

  return 0;
}
