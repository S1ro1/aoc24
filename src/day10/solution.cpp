#include "solution.hpp"

#include <iostream>
#include <print>
#include <map>
#include <set>


namespace Day10
{
  const std::pair<int, int> dirs[4] = {
    std::make_pair(0, 1),
    std::make_pair(1, 0),
    std::make_pair(0, -1),
    std::make_pair(-1, 0)
  };

  auto solve = [](std::string filename, bool distinct) -> int {
    auto grid = utils::parse_and_split_2d<int>(filename, "\n", "");

    std::queue<std::pair<int, int>> q;
    int total = 0;

    for (int i = 0; i < grid.size(); ++i) {
      for (int j = 0; j < grid[0].size(); ++j) {
        if (grid[i][j] == 0) {
          auto p = std::make_pair(i, j);
          q.push(p);
        }
      }
    }

    while (!q.empty()) {
      auto [row, col] = q.front();
      q.pop();

      std::queue<std::pair<int, int>> tmp;
      std::set<std::pair<int, int>> visited;
      tmp.push({row, col});

      while (!tmp.empty()) {
        auto [row, col] = tmp.front();
        auto v = grid[row][col];
        tmp.pop();

        for (const auto &[dx, dy]: dirs) {
          int nr = row + dx;
          int nc = col + dy;
          auto nv = grid[nr][nc];

          if (nr < 0 || nr >= grid.size() || nc < 0 || nc >= grid[0].size()) continue;
          if (nv != v + 1) continue;
          if (visited.find({nr, nc}) != visited.end() && !distinct) continue;

          visited.insert({nr, nc});

          if (nv == 9) {
            total += 1;
            continue;
          }
          tmp.push(std::make_pair(nr, nc));
        }
      }
    }
    return total;

  };

  int part1(const std::string &filename)
  {
    return solve(filename, false);
  }

  int part2(const std::string &filename)
  {
    return solve(filename, true);
  }
}
