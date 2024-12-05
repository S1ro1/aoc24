#include "solution.hpp"

#include <iostream>
#include <ostream>
#include <unordered_map>
#include <queue>

namespace Day5
{
  int part1(const std::string &filename)
  {
    auto data = utils::read_file_content(filename);

    auto split_data = utils::split_string(data, "\n\n");
    auto instructions = utils::parse_to_map<int, int, false, false>(split_data[0], "\n", "|");
    auto sequences = utils::parse_and_split_2d<int, false>(split_data[1], "\n", ",");

    auto total = 0;

    for (auto i = 0; i < sequences.size(); i++) {
      auto seq = sequences[i];
      const auto midElement = seq[seq.size() / 2];

      bool isValid = true;

      for (auto [k, v]: instructions) {
        const auto keyPos = std::find(seq.begin(), seq.end(), k);
        const auto valuePos = std::find(seq.begin(), seq.end(), v);
        if (valuePos == seq.end() || keyPos == seq.end()) continue;
        if (keyPos >= valuePos) {
          isValid = false;
        }
      }
      total += midElement * isValid;

    }
    return total;
  }

  int part2(const std::string &filename)
  {
    auto data = utils::read_file_content(filename);

    auto split_data = utils::split_string(data, "\n\n");
    auto instructions = utils::parse_to_map<int, int, false, false>(split_data[0], "\n", "|");
    auto sequences = utils::parse_and_split_2d<int, false>(split_data[1], "\n", ",");

    auto total = 0;

    for (auto i = 0; i < sequences.size(); i++) {
      auto seq = sequences[i];
      const auto midElement = seq[seq.size() / 2];

      std::vector<std::tuple<int, int>> validInstructions;

      bool isValid = true;

      for (auto [k, v]: instructions) {
        const auto keyPos = std::find(seq.begin(), seq.end(), k);
        const auto valuePos = std::find(seq.begin(), seq.end(), v);
        if (valuePos == seq.end() || keyPos == seq.end()) continue;

        validInstructions.emplace_back(k, v);

        if (keyPos >= valuePos) {
          isValid = false;
        }
      }

      if (!isValid){
        auto sortedSeq= utils::topological_sort<int>(validInstructions);

        total += sortedSeq[sortedSeq.size() / 2];
      }

    }
    return total;
  }
}
