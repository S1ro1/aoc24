#include "solution.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <ranges>

namespace Day1 {
  int part1(const std::string &filename) {
    const auto data = utils::parse_and_split_2d<int>(filename, "\n", " ");
    return std::ranges::fold_left(
      std::ranges::zip_view(
        ([](auto vec) {
          std::sort(vec.begin(), vec.end());
          return vec;
        })(data | std::views::transform([](const auto &row) { return row[0]; }) | std::ranges::to<std::vector>()) |
        std::views::all,
        ([](auto vec) {
          std::sort(vec.begin(), vec.end());
          return vec;
        })(data | std::views::transform([](const auto &row) { return row[1]; }) | std::ranges::to<std::vector>()) |
        std::views::all
      ) | std::views::transform([](const auto &r) { return std::abs(std::get<0>(r) - std::get<1>(r)); }),
      0,
      std::plus<>()
    );
  }

  int part2(const std::string &filename) {
    const auto data = utils::parse_and_split_2d<int>(filename, "\n", " ");
    return std::ranges::fold_left(
      (data | std::views::transform([](const auto& row) { return row[0]; }) | std::ranges::to<std::vector>()) |
      std::views::transform([v2 = data | std::views::transform([](const auto& row) { return row[1]; }) | std::ranges::to<std::vector>()]
        (const auto& val) { return val * std::count(v2.begin(), v2.end(), val); }),
      0,
      std::plus<>()
    );
  }
}
