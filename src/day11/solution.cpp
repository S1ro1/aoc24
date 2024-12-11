#include "solution.hpp"
#include <map>


namespace Day11

{
  using ull = unsigned long long;
  using State = std::pair<ull, int>; // stone, depth

  ull processStone(ull s, int depth, std::map<State, ull> &c, int maxDepth) {
      ull res;
      if (depth == maxDepth) return 1;

      if (c.find({s, depth}) != c.end()) return c[{s, depth}];

      if (s == 0) {
        res = processStone(s + 1, depth + 1, c, maxDepth);
        c[{s, depth}] = res;
        return res;
      }
      if (std::string x = std::to_string(s); x.length() % 2 == 0) {
        auto tmp = processStone(std::stoull(x.substr(0, x.length() / 2)), depth + 1, c, maxDepth);
        auto tmp2 = processStone(std::stoull(x.substr(x.length() / 2, x.length())), depth + 1, c, maxDepth);
        c[{s, depth}] = tmp + tmp2;;
        return tmp + tmp2;
      }
      res = processStone(s * 2024, depth + 1, c, maxDepth);
      c[{s, depth}] = res;
      return res;
  }

  ull part1(const std::string &filename)
  {
    int maxDepth = 25;
    auto ss = utils::parse_and_split_2d<int>(filename, "\n", " ")[0];
    std::map<State, ull> c;
    ull total = 0;
    for (auto &p : ss) {
      total += processStone(p, 0, c, maxDepth);
    }
    return total;
  }

  ull part2(const std::string &filename)
  {
    int maxDepth = 75;
    auto ss = utils::parse_and_split_2d<int>(filename, "\n", " ")[0];
    std::map<State, ull> c;
    ull total = 0;
    for (auto &p : ss) {
      total += processStone(p, 0, c, maxDepth);
    }
    return total;

  }
}
