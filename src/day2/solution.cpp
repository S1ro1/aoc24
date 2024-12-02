#include "solution.hpp"

#include <iostream>
#include <ostream>
#include <ranges>
#include <__algorithm/ranges_all_of.h>
#include <__algorithm/ranges_any_of.h>

namespace Day2 {
  auto test_sequence = [](auto &seq) {
    auto differences = std::views::iota(1u, seq.size())
                       | std::views::transform([&seq](size_t i) {
                         return seq[i] - seq[i - 1];
                       });

    bool all_increasing = std::ranges::all_of(differences, [](int diff) {
      return diff > 0 && diff <= 3;
    });

    bool all_decreasing = std::ranges::all_of(differences, [](int diff) {
      return diff < 0 && diff >= -3;
    });

    return all_increasing || all_decreasing;
  };

  int part1(const std::string &filename) {
    auto data = utils::parse_and_split_2d<int>(filename, "\n", " ");

    int total = 0;

    for (const auto &row: data) {
      total += test_sequence(row);
    }


    return total;
  }

  int part2(const std::string &filename) {
    auto data = utils::parse_and_split_2d<int>(filename, "\n", " ");

    int total = 0;

    for (auto &row: data) {
      if (test_sequence(row)) {
        total += 1;
        continue;
      }

      bool jump_out = false;

      for (size_t i = 0; i < row.size(); i++) {
        if (jump_out) break;

        std::vector<int> tmp;
        tmp.reserve(row.size() - 1);

        for (auto j = 0; j < row.size(); j++) {
          if (j != i) tmp.push_back(row[j]);
        }

        if (test_sequence(tmp)) {
          total += 1;
          jump_out = true;
        }
      }
    }

    return total;
  }
}
