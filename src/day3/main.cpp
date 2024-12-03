
#include <iostream>
#include <cassert>

#include "solution.hpp"

int main(int argc, char **argv)
{
  assert(argc == 2);

  auto result1 = Day3::part1(argv[1]);
  std::cout << "Part 1: " << result1 << std::endl;
  auto result2 = Day3::part2(argv[1]);
  std::cout << "Part 2: " << result2 << std::endl;
}
