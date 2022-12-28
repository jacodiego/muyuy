#pragma once

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cstdlib>

struct GridLocation
{
  int x, y;

  bool operator==(const GridLocation &b) const
  {
    return x == b.x && y == b.y;
  }

  bool operator!=(const GridLocation &b) const
  {
    return !(x == b.x && y == b.y);
  }

  bool operator<(const GridLocation &b) const
  {
    return std::tie(x, y) < std::tie(b.x, b.y);
  }

  std::basic_iostream<char>::basic_ostream &operator<<(std::basic_iostream<char>::basic_ostream &out)
  {
    out << '(' << x << ',' << y << ')';
    return out;
  }
};

namespace std
{
  /* implement hash function so we can put GridLocation into an unordered_set */
  template <>
  struct hash<GridLocation>
  {
    std::size_t operator()(const GridLocation &id) const noexcept
    {
      // NOTE: better to use something like boost hash_combine
      return std::hash<int>()(id.x ^ (id.y << 16));
    }
  };
}

class SquareGrid
{
public:
  SquareGrid();
  SquareGrid(int, int);
  void addWall(GridLocation);
  std::vector<GridLocation> neighbors(GridLocation) const;

private:
  constexpr static std::array<GridLocation, 4> DIRS = {
      /* East, West, North, South */
      GridLocation{1, 0}, GridLocation{-1, 0},
      GridLocation{0, -1}, GridLocation{0, 1}};
  ;

  int width, height;
  std::unordered_set<GridLocation> walls;
  bool in_bounds(GridLocation) const;

  bool passable(GridLocation) const;
};
