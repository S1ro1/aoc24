#include "solution.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <ranges>

namespace Day1
{
  int part1(const std::string &filename)
  {
    const auto data = utils::parse_and_split_2d<int>(filename, "\n", " ");

    const auto left = data | std::views::transform([](const auto &row)
                                                   { return row[0]; });
    const auto right = data | std::views::transform([](const auto &row)
                                                    { return row[1]; });

    std::vector<int> left_vec(left.begin(), left.end());
    std::vector<int> right_vec(right.begin(), right.end());

    std::sort(left_vec.begin(), left_vec.end());
    std::sort(right_vec.begin(), right_vec.end());


    std::vector<int> diffs;
    std::transform(left_vec.begin(), left_vec.end(), right_vec.begin(),
                   std::back_inserter(diffs),
                   [](const auto &l, const auto &r)
                   { return std::abs(l - r); });

    return std::accumulate(diffs.begin(), diffs.end(), 0);
  }

  int part2(const std::string &filename)
  {
    const auto data = utils::parse_and_split_2d<int>(filename, "\n", " ");

    const auto left = data | std::views::transform([](const auto &row)
                                                   { return row[0]; });
    const auto right = data | std::views::transform([](const auto &row)
                                                    { return row[1]; });
    
    std::vector<int> diffs;
    std::transform(left.begin(), left.end(), right.begin(),
                   std::back_inserter(diffs),
                   [&right](const auto &l, const auto &r)
                   { return l * std::count(right.begin(), right.end(), l); });

    return std::accumulate(diffs.begin(), diffs.end(), 0);

  }
}
