#include "solution.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <set>
#include <tuple> // for std::tuple

namespace Day6 {
  struct Guard {
    int col;
    int row;
    int dRow = -1;
    int dCol = 0;

    Guard(int col, int row) : col(col), row(row) {
    }
  };

  auto evalPos = [](Guard &guard, std::vector<std::string> data) -> bool {
    int col = guard.col;
    int row = guard.row;

    int dRow = guard.dRow;
    int dCol = guard.dCol;

    auto newCol = col + dCol;
    auto newRow = row + dRow;

    if (newCol >= data[0].size() || newRow >= data.size())
      return false;

    if (data[newRow][newCol] == '#') {
      guard.dRow = dCol;
      guard.dCol = -dRow;
    } else {
      guard.col = newCol;
      guard.row = newRow;
    }
    return true;
  };

  int part1(const std::string &filename) {
    auto data = utils::parse_and_split<std::string>(filename);

    auto guard = Guard(0, 0);
    std::vector<std::vector<bool> > visited{data.size(), std::vector<bool>(data[0].size(), false)};

    for (int r = 0; r < data.size(); r++) {
      for (int c = 0; c < data[0].size(); c++) {
        if (data[r][c] == '^') guard = Guard{c, r};
      }
    }

    while (1) {
      int col = guard.col;
      int row = guard.row;

      visited[row][col] = true;

      if (!evalPos(guard, data)) break;
    }

    int total = 0;
    for (auto &row: visited) {
      for (auto col: row) {
        if (col) total++;
      }
    }

    return total;
  }

  int part2(const std::string &filename) {
    auto data = utils::parse_and_split<std::string>(filename);
    auto guard = Guard(0, 0);

    for (int r = 0; r < data.size(); r++) {
      for (int c = 0; c < data[0].size(); c++) {
        if (data[r][c] == '^') guard = Guard{c, r};
      }
    }

    auto checkPosition = [&](int testRow, int testCol) -> bool {
      if (data[testRow][testCol] != '.') return false;

      auto testData = data;
      testData[testRow][testCol] = '#';

      auto testGuard = guard;
      using State = std::tuple<int, int, int, int>; // row, col, dRow, dCol
      std::set<State> visited;

      while (true) {
        auto state = std::make_tuple(
          testGuard.row,
          testGuard.col,
          testGuard.dRow,
          testGuard.dCol);

        if (visited.count(state)) return true;
        visited.insert(state);

        if (!evalPos(testGuard, testData)) return false;
      }
      return false;
    };

    const int threadCount = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::mutex mtx;
    int totalLoops = 0;

    auto processSection = [&](int startRow, int endRow) {
      int localCount = 0;
      for (int r = startRow; r < endRow; r++) {
        for (int c = 0; c < data[0].size(); c++) {
          if (checkPosition(r, c)) localCount++;
        }
      }
      std::lock_guard<std::mutex> lock(mtx);
      totalLoops += localCount;
    };

    int rowsPerThread = data.size() / threadCount;
    for (int i = 0; i < threadCount; i++) {
      int startRow = i * rowsPerThread;
      int endRow = (i == threadCount - 1) ? data.size() : (i + 1) * rowsPerThread;
      threads.emplace_back(processSection, startRow, endRow);
    }

    for (auto &thread: threads) thread.join();

    return totalLoops;
  }
}
