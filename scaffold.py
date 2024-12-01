#! /usr/bin/python3

import os
from argparse import ArgumentParser


MAIN_CONTENT = """
#include <iostream>
#include <cassert>

#include "solution.hpp"

int main(int argc, char **argv)
{{
  assert(argc == 2);

  auto result1 = Day{day}::part1(argv[1]);
  std::cout << "Part 1: " << result1 << std::endl;
  auto result2 = Day{day}::part2(argv[1]);
  std::cout << "Part 2: " << result2 << std::endl;
}}
"""

SOLUTION_CONTENT = """#include "solution.hpp"

namespace Day{day}
{{
  int part1(const std::string &filename)
  {{
    return 0;
  }}

  int part2(const std::string &filename)
  {{
    return 0;
  }}
}}
"""

SOLUTION_HEADER_CONTENT = """#pragma once

#include <string>

import utils;

namespace Day{day}
{{
  int part1(const std::string &filename);
  int part2(const std::string &filename);
}}
"""


def scaffold_directory(day: int, force: bool = False):
    # Ensure src directory exists
    os.makedirs("src", exist_ok=True)
    os.makedirs(
        "src/utils", exist_ok=True
    )  # Create utils directory if it doesn't exist

    # Create day directory inside src
    day_dir = f"src/day{day}"
    os.makedirs(day_dir, exist_ok=True)
    os.makedirs(f"{day_dir}/data", exist_ok=True)

    # Create empty data files
    open(f"{day_dir}/data/data.txt", "a").close()
    open(f"{day_dir}/data/test.txt", "a").close()

    # Create source files
    if not os.path.exists(f"{day_dir}/main.cpp") or force:
        with open(f"{day_dir}/main.cpp", "w") as f:
            f.write(MAIN_CONTENT.format(day=day))

    if not os.path.exists(f"{day_dir}/solution.cpp") or force:
        with open(f"{day_dir}/solution.cpp", "w") as f:
            f.write(SOLUTION_CONTENT.format(day=day))

    if not os.path.exists(f"{day_dir}/solution.hpp") or force:
        with open(f"{day_dir}/solution.hpp", "w") as f:
            f.write(SOLUTION_HEADER_CONTENT.format(day=day))

    # Regenerate CMake
    build_dir = "build"
    if os.path.exists(build_dir):
        os.system(f"cmake -B {build_dir}")


def main():
    parser = ArgumentParser()
    parser.add_argument("day", type=int)
    parser.add_argument("--force", action="store_true")
    args = parser.parse_args()

    scaffold_directory(args.day, force=args.force)

    print(f"Created directory for day {args.day}!")


if __name__ == "__main__":
    main()
