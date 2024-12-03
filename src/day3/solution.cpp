#include <iostream>

#include "solution.hpp"

#include <regex>

namespace Day3
{
  std::regex pattern(R"(mul\((\d+),(\d+)\))");

  int part1(const std::string &filename)
  {
    auto input = utils::read_file_content(filename, "");

    std::smatch matches;

    auto str = input;

    int total = 0;

    while (std::regex_search(str, matches, pattern))
    {
      int num1 = std::stoi(matches[1].str());
      int num2 = std::stoi(matches[2].str());

      str = matches.suffix();
      total += num1 * num2;
    }

    return total;
  }

  int part2(const std::string &filename)
  {
    auto input = utils::read_file_content(filename, "");

    std::regex enablePattern(R"(do\(\)|don't\(\))");

    std::smatch matches{};
    std::smatch enableMatches{};

    std::string str = input;
    int total = 0;
    bool enabled = true;

    while (std::regex_search(str, enableMatches, enablePattern))
    {
      auto prefix = str.substr(0, enableMatches.position());
      while (std::regex_search(prefix, matches, pattern))
      {
        if (enabled)
          total += std::stoi(matches[1].str()) * std::stoi(matches[2].str());
        prefix = matches.suffix();
      }

      enabled = (enableMatches[0] == "do()");
      str = enableMatches.suffix();
    }

    while (std::regex_search(str, matches, pattern) && enabled)
    {
      total += std::stoi(matches[1].str()) * std::stoi(matches[2].str());
      str = matches.suffix();
    }

    return total;
  }
}
