#include "solution.hpp"

#include <iostream>
#include <vector>

namespace Day7
{
  auto generateCombinations = [](int length, int min, int max) {
    std::vector<std::vector<int>> result;
    std::vector<int> v(length, min);
    bool done = false;

    while (!done) {
      result.push_back(v);

      int pos = length - 1;
      while (pos >= 0) {
        if (v[pos] < max) {
          v[pos]++;
          break;
        }
        v[pos] = min;
        pos--;
      }
      if (pos < 0) done = true;
    }

    return result;
  };

  auto evalExpression = [](long result, std::vector<int> ops, std::vector<long> nums) -> bool {
    auto init = nums[0];

    for (int i = 1; i < nums.size(); i++) {
      switch (ops[i - 1]) {
        case 0:
          init = init + nums[i];
        break;
        case 1:
          init = init * nums[i];
        break;
        case 2:
          init = std::stol(std::to_string(init) + std::to_string(nums[i]));
        break;
        default:;
      }
    }

    return init == result;
  };

  long part1(const std::string &filename) {
    auto data = utils::parse_to_map<long, std::vector<long>, true, false>(filename);
    long result = 0;

    for (const auto &[k, v] : data) {
      auto combs = generateCombinations(v.size() - 1, 0, 1);
      bool done = false;

      for (const auto &ops : combs) {
        done = evalExpression(k, ops, v);
        if (done) break;
      }

      result += done * k;
    }
    return result;
  }

  long part2(const std::string &filename) {
    auto data = utils::parse_to_map<long, std::vector<long>, true, false>(filename);
    long result = 0;

    for (const auto &[k, v] : data) {
      auto combs = generateCombinations(v.size() - 1, 0, 2);
      bool done = false;

      for (const auto &ops : combs) {
        done = evalExpression(k, ops, v);
        if (done) break;
      }

      result += done * k;
    }
    return result;
  }
}
