#include <iostream>
#include "solution.hpp"

namespace Day4
{

  struct Direction
  {
    int x;
    int y;
    Direction(int x, int y) : x(x), y(y) {};
    auto operator*(int n) const
    {
      return Direction(x * n, y * n);
    }
  };

  const Direction dirs[8] = {
      {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

  const char i2c[3] = {'M', 'A', 'S'};

  bool checkPoint(int x, int y, Direction dir, auto &board, int i)
  {
    int new_x = x + dir.x * i;
    int new_y = y + dir.y * i;

    if (new_x < 0 || new_x >= board.size() || new_y < 0 || new_y >= board[new_x].size())
      return false;

    return board[new_x][new_y] == i2c[i - 1];
  }

  int part1(const std::string &filename)
  {
    int result = 0;
    auto data = utils::parse_and_split<std::string>(filename, "\n", "");

    for (int row = 0; row < data.size(); row++)
    {
      for (int col = 0; col < data[row].size(); col++)
      {
        if (data[row][col] != 'X')
          continue;

        for (const auto &dir : dirs)
        {
          bool found = true;
          for (int i = 1; i < 4; i++)
          {
            if (!checkPoint(row, col, dir, data, i))
            {
              found = false;
              break;
            }
          }

          result += found;
        }
      }
    }

    return result;
  }

  const Direction upper[2] = {
      {-1, -1}, {1, 1}};
  const Direction lower[2] = {
      {-1, 1}, {1, -1}};

  char getPoint(int x, int y, Direction dir, auto &board)
  {
    int new_x = x + dir.x;
    int new_y = y + dir.y;

    if (new_x < 0 || new_x >= board.size() || new_y < 0 || new_y >= board[new_x].size())
      return false;

    return board[new_x][new_y];
  }

  int part2(const std::string &filename)
  {
    int result = 0;
    auto data = utils::parse_and_split<std::string>(filename, "\n", "");

    for (int row = 0; row < data.size(); row++)
    {
      for (int col = 0; col < data[row].size(); col++)
      {
        if (data[row][col] != 'A')
          continue;

        if (getPoint(row, col, upper[0], data) == 'M' && getPoint(row, col, upper[1], data) == 'S' &&
            getPoint(row, col, lower[0], data) == 'M' && getPoint(row, col, lower[1], data) == 'S')
        {
          result++;
        }

        if (getPoint(row, col, upper[0], data) == 'S' && getPoint(row, col, upper[1], data) == 'M' &&
            getPoint(row, col, lower[0], data) == 'S' && getPoint(row, col, lower[1], data) == 'M')
        {
          result++;
        }

        if (getPoint(row, col, upper[0], data) == 'S' && getPoint(row, col, upper[1], data) == 'M' &&
            getPoint(row, col, lower[0], data) == 'M' && getPoint(row, col, lower[1], data) == 'S')
        {
          result++;
        }

        if (getPoint(row, col, upper[0], data) == 'M' && getPoint(row, col, upper[1], data) == 'S' &&
            getPoint(row, col, lower[0], data) == 'S' && getPoint(row, col, lower[1], data) == 'M')
        {
          result++;
        }
      }
    }

    return result;
  }
}
