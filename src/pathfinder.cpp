#include "pathfinder.hpp"
#include <cmath>
#include <chrono>
#include <queue>
#include <unordered_map>
#include <algorithm>

namespace na 
{
     pathfinder::pathfinder() 
     {
          a_star_grid.resize(grid_rows, std::vector<cell>(grid_cols));
          dijkstra_grid.resize(grid_rows, std::vector<cell>(grid_cols));

          for (int y = 0; y < grid_rows; y++) 
          {
               for (int x = 0; x < grid_cols; x++) 
               {
                    a_star_grid[y][x] = cell(x, y);
                    dijkstra_grid[y][x] = cell(x, y);
               }
          }
     }

     void pathfinder::update() 
     {
          Vector2 mouse_pos = GetMousePosition();
          int grid_x = static_cast<int>(mouse_pos.x) / cell_size;
          int grid_y = static_cast<int>(mouse_pos.y) / cell_size;
          bool is_left_side = mouse_pos.x < static_cast<float>(screen_width) / 2;

          if (grid_x >= 0 && grid_x < grid_cols && grid_y >= 0 && grid_y < grid_rows) 
          {
               auto& current_grid = is_left_side ? a_star_grid : dijkstra_grid;
               grid_x = is_left_side ? grid_x : grid_x - grid_cols;

               if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
               {
                    cell& current_cell = current_grid[grid_y][grid_x];
                    cell& mirror_cell = is_left_side ? dijkstra_grid[grid_y][grid_x]
                         : a_star_grid[grid_y][grid_x];

                    if (!start_cell_a_star && current_cell.type != cell_type::END) 
                    {
                         current_cell.type = cell_type::START;
                         mirror_cell.type = cell_type::START;
                         start_cell_a_star = &a_star_grid[grid_y][grid_x];
                         start_cell_dijkstra = &dijkstra_grid[grid_y][grid_x];
                    }
                    else if (!end_cell_a_star && current_cell.type != cell_type::START) 
                    {
                         current_cell.type = cell_type::END;
                         mirror_cell.type = cell_type::END;
                         end_cell_a_star = &a_star_grid[grid_y][grid_x];
                         end_cell_dijkstra = &dijkstra_grid[grid_y][grid_x];
                    }
                    else {
                         is_placing_walls = true;
                    }
               }

               // Place walls when mouses clicked and draged
               if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && is_placing_walls)
               {
                    if (grid_x >= 0 && grid_x < grid_cols) {
                         if (current_grid[grid_y][grid_x].type != cell_type::START &&
                              current_grid[grid_y][grid_x].type != cell_type::END) {
                              current_grid[grid_y][grid_x].type = cell_type::WALL;
                              auto& mirror_grid = is_left_side ? dijkstra_grid : a_star_grid;
                              mirror_grid[grid_y][grid_x].type = cell_type::WALL;
                         }
                    }
               }
          }

          if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
          {
               is_placing_walls = false;
          }

          
          if (IsKeyPressed(KEY_SPACE)) 
          {
                    a_star_stats = find_path(a_star_grid, start_cell_a_star, end_cell_a_star, true);
                    dijkstra_stats = find_path(dijkstra_grid, start_cell_dijkstra, end_cell_dijkstra,
                         false);

                    accum_time -= 1.0f;
               
          }

          if (IsKeyPressed(KEY_R)) 
          {
               for (int y = 0; y < grid_rows; y++)
               {
                    for (int x = 0; x < grid_cols; x++) 
                    {
                         a_star_grid[y][x] = cell(x, y);
                         dijkstra_grid[y][x] = cell(x, y);
                    }
               }
               start_cell_a_star = start_cell_dijkstra = nullptr;
               end_cell_a_star = end_cell_dijkstra = nullptr;
               a_star_stats = path_stats();
               dijkstra_stats = path_stats();
          }
     }

     void pathfinder::render()
     {
          DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

          // Render Lamda
          auto draw_grid = [this](const grid& grid, int offset_x) 
               {
               for (int y = 0; y < grid_rows; y++) {
                    for (int x = 0; x < grid_cols; x++) {
                         Rectangle cell_rect = {
                             static_cast<float>(x * cell_size + offset_x),
                             static_cast<float>(y * cell_size),
                             static_cast<float>(cell_size),
                             static_cast<float>(cell_size)
                         };

                         Color cell_color;
                         switch (grid[y][x].type) {
                         case cell_type::WALL:
                              cell_color = { 100, 100, 100, 255 };
                              break;
                         case cell_type::START:
                              cell_color = GREEN;
                              break;
                         case cell_type::END:
                              cell_color = RED;
                              break;
                         case cell_type::PATH:
                              cell_color = YELLOW;
                              break;
                         case cell_type::VISITED:
                              cell_color = get_visited_color(grid[y][x].visit_order);
                              break;
                         default:
                              cell_color = BLACK;
                              break;
                         }

                         DrawRectangleRec(cell_rect, cell_color);
                         DrawRectangleLinesEx(cell_rect, 1, { 100, 100, 100, 50 });
                    }
               }
               };

          
               draw_grid(a_star_grid, 0);
               draw_grid(dijkstra_grid, screen_width / 2);
           
          

          // Draw A* algorithm statistics
          DrawText("A* Algorithm", 10, 10, 20, SKYBLUE);
          DrawText(("Time: " + std::to_string(a_star_stats.execution_time) + " ms").c_str(),
               10, 35, 20, SKYBLUE);
          DrawText(("Nodes visited: " + std::to_string(a_star_stats.nodes_visited)).c_str(),
               10, 60, 20, SKYBLUE);
          DrawText(("Path length: " + std::to_string(a_star_stats.path_length)).c_str(),
               10, 85, 20, SKYBLUE);

          // Vertical partition
          DrawRectangle(screen_width / 2, 0, 5, screen_height, BLACK);

          // Draw Dijkstra algorithm statistics
          DrawText("Dijkstra's Algorithm", screen_width / 2 + 30, 10, 20, YELLOW);
          DrawText(("Time: " + std::to_string(dijkstra_stats.execution_time) + " ms").c_str(),
               screen_width / 2 + 30, 35, 20, YELLOW);
          DrawText(("Nodes visited: " + std::to_string(dijkstra_stats.nodes_visited)).c_str(),
               screen_width / 2 + 30, 60, 20, YELLOW);
          DrawText(("Path length: " + std::to_string(dijkstra_stats.path_length)).c_str(),
               screen_width / 2 + 30, 85, 20, YELLOW);

          // Draw user instructions
          DrawText("Press SPACE to find path", 10, screen_height - 60, 20, DARKGRAY);
          DrawText("Press R to reset", 10, screen_height - 30, 20, DARKGRAY);
     }

     float pathfinder::calculate_heuristic(const cell* current, const cell* end) const
     {
          return static_cast<float>(std::sqrt(
               std::pow(current->x - end->x, 2) +
               std::pow(current->y - end->y, 2)
          ));
     }

     std::vector<cell*> pathfinder::get_neighbors(cell* current, grid& grid)
     {
          std::vector<cell*> neighbors;

          // Directions including diagonals (8 directions)
          // didn't wanted to use std::array<T,S> 
          const int dx[] = { -1, 1, 0, 0, -1, -1, 1, 1 };
          const int dy[] = { 0, 0, -1, 1, -1, 1, -1, 1 };

          for (int i = 0; i < 8; i++) {
               int new_x = current->x + dx[i];
               int new_y = current->y + dy[i];

               // Check grid bounds
               if (new_x >= 0 && new_x < grid_cols && new_y >= 0 && new_y < grid_rows) 
               {
                    // Extra check for diagonal movement to prevent "cutting corners"
                    if (i >= 4) {
                         int adj_x1 = current->x + dx[i];
                         int adj_y1 = current->y;
                         int adj_x2 = current->x;
                         int adj_y2 = current->y + dy[i];

                         // Skip diagonal if adjacent cells are walls
                         if (grid[adj_y1][adj_x1].type == cell_type::WALL ||
                              grid[adj_y2][adj_x2].type == cell_type::WALL) 
                         {
                              continue;
                         }
                    }

                    // Add neighbor if not a wall
                    if (grid[new_y][new_x].type != cell_type::WALL) 
                    {
                         neighbors.push_back(&grid[new_y][new_x]);
                    }
               }
          }
          return neighbors;
     }

     Color pathfinder::get_visited_color(int visit_order) const
     {
          float progress = static_cast<float>(visit_order) / total_visited_nodes;

          // Define three colors for the gradient
          Color blue = { 25, 25, 255, 200 };      // Deep blue
          Color violet = { 138, 43, 226, 200 };   // Violet
          Color pink = { 255, 20, 100, 200 };     // Hot pink

          Color result;
          if (progress < 0.5f) {
               // First half: interpolate between blue and violet
               float adjusted_progress = progress * 2.0f;
               result = Color{
                   static_cast<unsigned char>(blue.r + (violet.r - blue.r) * adjusted_progress),
                   static_cast<unsigned char>(blue.g + (violet.g - blue.g) * adjusted_progress),
                   static_cast<unsigned char>(blue.b + (violet.b - blue.b) * adjusted_progress),
                   255
               };
          }
          else {
               // Second half: interpolate between violet and pink
               float adjusted_progress = (progress - 0.5f) * 2.0f;
               result = Color{
                   static_cast<unsigned char>(violet.r + (pink.r - violet.r) * adjusted_progress),
                   static_cast<unsigned char>(violet.g + (pink.g - violet.g) * adjusted_progress),
                   static_cast<unsigned char>(violet.b + (pink.b - violet.b) * adjusted_progress),
                   255
               };
          }

          return result;
     }

     path_stats pathfinder::find_path(grid& grid, cell* start, cell* end, bool use_a_star)
     {
          auto start_time = std::chrono::high_resolution_clock::now();
          path_stats stats;
          total_visited_nodes = 0;

          // Invalid start or end cells
          if (!start || !end) return stats;

          // Reset grid cells
          for (auto& row : grid) {
               for (auto& grid_cell : row) {
                    if (grid_cell.type == cell_type::PATH || grid_cell.type == cell_type::VISITED) {
                         grid_cell.type = cell_type::EMPTY;
                    }
                    grid_cell.g_cost = INF;
                    grid_cell.h_cost = 0;
                    grid_cell.f_cost = INF;
                    grid_cell.parent = nullptr;
                    grid_cell.visit_order = 0;
               }
          }

          // Custom priority queue comparator
          auto compare = [](cell* a, cell* b) {
               if (std::abs(a->get_f_cost() - b->get_f_cost()) < 0.0001f) {
                    return a->h_cost > b->h_cost;
               }
               return a->get_f_cost() > b->get_f_cost();
               };

          std::priority_queue<cell*, std::vector<cell*>, decltype(compare)> open_set(compare);
          std::unordered_map<cell*, bool> in_open_set;
          std::vector<cell*> closed_set;

          // Initialize start cell
          start->g_cost = 0;
          if (use_a_star) {
               start->h_cost = calculate_heuristic(start, end);
          }
          start->f_cost = start->get_f_cost();
          open_set.push(start);
          in_open_set[start] = true;

          // Main pathfinding loop
          while (!open_set.empty()) {
               cell* current = open_set.top();
               open_set.pop();
               in_open_set[current] = false;

               // Path found
               if (current == end) {
                    cell* path_cell = end;
                    while (path_cell != nullptr) {
                         if (path_cell != end && path_cell != start) {
                              path_cell->type = cell_type::PATH;
                              stats.path_length++;
                         }
                         stats.path.push_back(path_cell);
                         path_cell = path_cell->parent;
                    }
                    std::reverse(stats.path.begin(), stats.path.end());
                    break;
               }

               closed_set.push_back(current);

               // Mark cell as visited (except start/end)
               if (current != start && current != end) {
                    current->type = cell_type::VISITED;
                    current->visit_order = ++total_visited_nodes;
                    stats.nodes_visited++;
               }

               // Explore neighbors
               for (cell* neighbor : get_neighbors(current, grid)) {
                    if (std::find(closed_set.begin(), closed_set.end(), neighbor) != closed_set.end()) {
                         continue;
                    }

                    // Calculate movement cost (diagonal vs straight)
                    float tentative_g_cost = current->g_cost +
                         (std::abs(current->x - neighbor->x) + std::abs(current->y - neighbor->y) == 2 ?
                              1.414f : 1.0f);

                    // Update neighbor if better path found
                    if (tentative_g_cost < neighbor->g_cost) {
                         neighbor->parent = current;
                         neighbor->g_cost = tentative_g_cost;
                         if (use_a_star) {
                              neighbor->h_cost = calculate_heuristic(neighbor, end);
                         }
                         neighbor->f_cost = neighbor->get_f_cost();

                         if (!in_open_set[neighbor]) {
                              open_set.push(neighbor);
                              in_open_set[neighbor] = true;
                         }
                    }
               }
          }

          // Calculate execution time
          auto end_time = std::chrono::high_resolution_clock::now();
          stats.execution_time = std::chrono::duration<float>(end_time - start_time).count() * 1000.0f;
          return stats;
     }

   
}

