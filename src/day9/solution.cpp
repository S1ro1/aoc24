#include "solution.hpp"

#include <iostream>
#include <ranges>
#include <algorithm>

namespace Day9 {
  namespace rng = std::ranges;

  auto createImage = [](std::string filesystem) {
    std::vector<int> image;

    int max_id = 0;
    for (int i = 0; i < filesystem.size(); ++i) {
      auto val = filesystem[i] - '0';
      int id;
      if (i % 2 == 0) {
        id = i / 2;
        max_id = id;
      } else {
        id = -1;
      }
      for (int j = 0; j < val; ++j) image.push_back(id);
    }
    return std::make_pair(image, max_id);
  };

  auto findLastPositiveSequence = [](std::span<int> seq, int targetId) {
    auto it = seq.begin();
    auto end = seq.end();

    while (it != end && *it != targetId) ++it;
    if (it == end) return std::make_pair(end, end);

    auto seqStart = it;
    while (it != end && *it == targetId) ++it;

    return std::make_pair(seqStart, it);
  };

  auto findFirstNegativeSequenceWithLength = [](std::span<int> seq, int n) {
    if (seq.size() < n) return seq.end();

    for (auto it = seq.begin(); it <= seq.end() - n; ++it) {
      if (std::all_of(it, it + n, [](int x) { return x == -1; })) {
        return it;
      }
    }
    return seq.end();
  };


  long part1(const std::string &filename) {
    auto data = utils::parse_and_split<std::string>(filename);
    auto filesystem = data[0];

    auto image = createImage(filesystem).first;

    while (1) {
      auto firstNegative = rng::find(image, -1);
      auto lastPositive = rng::find_last_if(image, [](int i) { return i >= 0; });

      if (firstNegative > lastPositive.begin()) break;

      rng::iter_swap(firstNegative, lastPositive.begin());
    }

    long total = 0;
    for (int i = 0; i < image.size(); ++i) {
      if (image[i] == -1) break;
      total += i * image[i];
    }

    return total;
  }

  long part2(const std::string &filename) {
    namespace rng = std::ranges;
    auto data = utils::parse_and_split<std::string>(filename);
    auto filesystem = data[0];

    auto [image, maxId] = createImage(filesystem);

    std::span<int> tmp(image);
    for (int currentId = maxId; currentId >= 0; --currentId) {
      auto sequence = findLastPositiveSequence(tmp, currentId);
      if (sequence.first == tmp.end()) continue;

      auto length = sequence.second - sequence.first;
      auto firstNegative = findFirstNegativeSequenceWithLength(tmp, length);

      if (firstNegative != tmp.end() && firstNegative < sequence.first) {
        std::fill(firstNegative, firstNegative + length, currentId);
        std::fill(sequence.first, sequence.second, -1);
      }
    }
    long total = 0;
    for (int i = 0; i < image.size(); ++i) {
      if (image[i] == -1) continue;
      total += i * image[i];
    }
    return total;
  }
}
