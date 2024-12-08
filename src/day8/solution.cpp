#include "solution.hpp"

#include <string>
#include <vector>
#include <map>

namespace Day8
{
  using Position = std::pair<int, int>;

  auto evalPair = [](auto grid, const Position &pos, const Position &nextPos) -> std::pair<std::optional<Position>, std::optional<Position>> {
    auto dx = nextPos.first - pos.first;
    auto dy = nextPos.second - pos.second;

    auto x1 = pos.first + 2 * dx;
    auto y1 = pos.second + 2 * dy;

    auto x2 = nextPos.first - 2 * dx;
    auto y2 = nextPos.second - 2 * dy;

    std::optional<Position> a1;
    std::optional<Position> a2;

    if (x1 < grid.size() && y1 < grid[0].size()) {
      a1 = std::make_optional(std::make_pair(x1, y1));
    }
    if (x2 < grid.size() && y2 < grid[0].size()) {
      a2 = std::make_optional(std::make_pair(x2, y2));
    }

    return std::make_pair(a1, a2);
  };

  auto evalPair2 = [](auto &grid, auto &visited, const Position &pos, const Position &nextPos) -> int {

    auto result = 0;

    auto dx = nextPos.first - pos.first;
    auto dy = nextPos.second - pos.second;

    auto x1 = pos.first;
    auto y1 = pos.second;
    while (1) {
      x1 += dx;
      y1 += dy;

      if (x1 >= grid.size() || y1 >= grid[0].size() || x1 < 0 || y1 < 0) break;

      result += !visited[x1][y1] * 1;
      visited[x1][y1] = true;
    }

    auto x2 = nextPos.first;
    auto y2 = nextPos.second;
    while (1) {
      x2 -= dx;
      y2 -= dy;

      if (x2 >= grid.size() || y2 >= grid[0].size() || x2 < 0 || y2 < 0) break;

      result += !visited[x2][y2] * 1;
      visited[x2][y2] = true;
    }

    return result;
  };

  auto getAntennas = [](auto &grid) -> std::map<char, std::vector<Position>> {
    std::map<char, std::vector<Position>> positions;

    for (int row = 0; row < grid.size(); row++) {
      for (int col = 0; col < grid[0].size(); col++) {
        auto c = grid[row][col];
        if (c == '.') continue;
        positions[c].emplace_back(row, col);
      }
    }
    return positions;

  };

  int part1(const std::string &filename)
  {
    auto grid = utils::parse_and_split<std::string>(filename, "\n");
    auto positions = getAntennas(grid);

    int result = 0;
    std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));

    for (const auto &freq: positions) {
      auto antennas = freq.second;
      for (int i = 0; i < antennas.size(); i++) {
        for (int j = i + 1; j < antennas.size(); j++) {
          auto [a1, a2] = evalPair(grid, antennas[i], antennas[j]);
          if (a1 != std::nullopt) {
            result += !visited[a1->first][a1->second] * 1;
            visited[a1->first][a1->second] = true;
          }
          if (a2 != std::nullopt) {
            result += !visited[a2->first][a2->second] * 1;
            visited[a2->first][a2->second] = true;
          }
        }
      }
    }
    return result;
  }

  int part2(const std::string &filename)
  {
    auto grid = utils::parse_and_split<std::string>(filename, "\n");
    auto positions = getAntennas(grid);

    int result = 0;
    std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));

    for (const auto &freq: positions) {
      auto antennas = freq.second;
      for (int i = 0; i < antennas.size(); i++) {
        for (int j = i + 1; j < antennas.size(); j++) {
          result += evalPair2(grid, visited, antennas[i], antennas[j]);
        }
      }
    }
    return result;
  }
}
