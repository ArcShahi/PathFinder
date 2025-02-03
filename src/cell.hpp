#ifndef CELL_HPP
#define CELL_HPP

#include <vector>
#include <limits>

namespace na 
{
     enum cell_type 
     {
          EMPTY,
          WALL,
          START,
          END,
          PATH,
          VISITED
     };

     constexpr float INF = std::numeric_limits<float>::infinity();

     struct cell 
     {
          int x{};
          int y{};
          cell_type type{ EMPTY };
          float g_cost{ INF };
          float h_cost{ 0 };            // for A* 
          float f_cost{ INF };
          cell* parent{ nullptr };
          int visit_order{ 0 };

          cell() = default;
          cell(int _x, int _y) : x(_x), y(_y) {}

          float get_f_cost() const { return g_cost + h_cost; }
     };

     struct path_stats 
     {
          float execution_time{};
          int nodes_visited{};
          int path_length{};
          std::vector<cell*> path{};
     };
}

#endif // CELL_HPP