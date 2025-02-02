#include "raylib.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <string>
#include <limits>

constexpr int screen_width = 1920;
constexpr int screen_height = 1000;
constexpr int cell_size = 15;
constexpr int grid_cols = (screen_width / 2) / cell_size;
constexpr int grid_rows = screen_height / cell_size;

// Enum to represent different cell types in the grid
enum cell_type
{
     EMPTY,
     WALL,
     START,
     END,
     PATH,
     VISITED
};

// Structure to represent individual grid cell
struct cell
{
     int x, y;                       // Cell coordinates
     cell_type type;                 // Type of cell
     float g_cost;                   // Cost from start node
     float h_cost;                   // Heuristic cost to end node
     float f_cost;                   // Total estimated cost (g_cost + h_cost)
     cell* parent;                   // Parent cell for path reconstruction
     int visit_order;                // Order of cell visitation for gradient coloring

     // Constructor with default initialization
     cell(int x_pos = 0, int y_pos = 0) 
          :x(x_pos),y(y_pos),
          type(EMPTY),
          g_cost(std::numeric_limits<float>::infinity()),
          h_cost(0),
          f_cost(std::numeric_limits<float>::infinity()),
          parent(nullptr),
          visit_order(0) {
                               }

     // Calculate total path cost
     float get_f_cost() const { return g_cost + h_cost; }
};

// Structure to store pathfinding algorithm statistics
struct path_stats
{
     float execution_time = 0.0f;    // Time taken to find path
     int nodes_visited = 0;          // Number of nodes explored
     int path_length = 0;            // Length of final path
     std::vector<cell*> path;        // Actual path cells
};

class pathfinding_visualizer
{
private:
     // Grids for A* and Dijkstra algorithms
     std::vector<std::vector<cell>> grid_a_star;
     std::vector<std::vector<cell>> grid_dijkstra;

     // Start and end cells for both algorithms
     cell* start_cell_a_star = nullptr;
     cell* end_cell_a_star = nullptr;
     cell* start_cell_dijkstra = nullptr;
     cell* end_cell_dijkstra = nullptr;

     bool is_placing_walls = false;
     path_stats a_star_stats;
     path_stats dijkstra_stats;
     int total_visited_nodes = 0;

     // Calculate heuristic distance between two cells
     float calculate_heuristic(const cell* current_cell, const cell* end_cell)
     {
          return static_cast<float>(std::sqrt(
               std::pow(current_cell->x - end_cell->x, 2) +
               std::pow(current_cell->y - end_cell->y, 2)
          ));
     }

     // Get neighboring cells with advanced movement rules
     std::vector<cell*> get_neighbors(cell* current_cell, std::vector<std::vector<cell>>& grid)
     {
          std::vector<cell*> neighbors;
          // Directions including diagonals (8 directions)
          const int dx[] = { -1, 1, 0, 0, -1, -1, 1, 1 };
          const int dy[] = { 0, 0, -1, 1, -1, 1, -1, 1 };

          for (int i = 0; i < 8; i++) {
               int new_x = current_cell->x + dx[i];
               int new_y = current_cell->y + dy[i];

               // Check grid bounds
               if (new_x >= 0 && new_x < grid_cols && new_y >= 0 && new_y < grid_rows) {
                    // Extra check for diagonal movement to prevent "cutting corners"
                    if (i >= 4) {
                         int adj_x1 = current_cell->x + dx[i];
                         int adj_y1 = current_cell->y;
                         int adj_x2 = current_cell->x;
                         int adj_y2 = current_cell->y + dy[i];

                         // Skip diagonal if adjacent cells are walls
                         if (grid[adj_y1][adj_x1].type == WALL || grid[adj_y2][adj_x2].type == WALL) {
                              continue;
                         }
                    }

                    // Add neighbor if not a wall
                    if (grid[new_y][new_x].type != WALL) {
                         neighbors.push_back(&grid[new_y][new_x]);
                    }
               }
          }
          return neighbors;
     }

     // Generate visited Colors
     Color get_visited_color(int visit_order)const {
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
     // Core pathfinding algorithm implementation (A* and Dijkstra)
     path_stats find_path(std::vector<std::vector<cell>>& grid, cell* start, cell* end,
          bool use_a_star)
     {
          auto start_time = std::chrono::high_resolution_clock::now();
          path_stats stats;
          total_visited_nodes = 0;

          // Invalid start or end cells
          if (!start || !end) return stats;

          // Reset grid cells
          for (auto& row : grid)
          {
               for (auto& grid_cell : row)
               {
                    if (grid_cell.type == PATH || grid_cell.type == VISITED) {
                         grid_cell.type = EMPTY;
                    }
                    grid_cell.g_cost = std::numeric_limits<float>::infinity();
                    grid_cell.h_cost = 0;
                    grid_cell.f_cost = std::numeric_limits<float>::infinity();
                    grid_cell.parent = nullptr;
                    grid_cell.visit_order = 0;
               }
          }

          // Custom priority queue comparator
          auto compare = [](cell* a, cell* b) {
               if (std::abs(a->get_f_cost() - b->get_f_cost()) < 0.0001f)
               {
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
          while (!open_set.empty())
          {
               cell* current = open_set.top();
               open_set.pop();
               in_open_set[current] = false;

               // Path found
               if (current == end) {
                    cell* path_cell = end;
                    while (path_cell != start)
                    {
                         if (path_cell != end)
                         {
                              path_cell->type = PATH;
                              stats.path_length++;
                         }
                         stats.path.push_back(path_cell);
                         path_cell = path_cell->parent;
                    }
                    stats.path.push_back(start);
                    std::reverse(stats.path.begin(), stats.path.end());
                    break;
               }

               closed_set.push_back(current);

               // Mark cell as visited (except start/end)
               if (current != start && current != end)
               {
                    current->type = VISITED;
                    current->visit_order = ++total_visited_nodes;
                    stats.nodes_visited++;
               }

               // Explore neighbors
               for (cell* neighbor : get_neighbors(current, grid))
               {
                    if (std::find(closed_set.begin(), closed_set.end(), neighbor) != closed_set.end())
                    {
                         continue;
                    }

                    // Calculate movement cost (diagonal vs straight)
                    float tentative_g_cost = current->g_cost +
                         (std::abs(current->x - neighbor->x) + std::abs(current->y - neighbor->y) == 2 ?
                              1.414f : 1.0f);

                    // Update neighbor if better path found
                    if (tentative_g_cost < neighbor->g_cost)
                    {
                         neighbor->parent = current;
                         neighbor->g_cost = tentative_g_cost;
                         if (use_a_star)
                         {
                              neighbor->h_cost = calculate_heuristic(neighbor, end);
                         }
                         neighbor->f_cost = neighbor->get_f_cost();

                         if (!in_open_set[neighbor])
                         {
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

public:
     // Constructor: Initialize grids
     pathfinding_visualizer() {
          grid_a_star.resize(grid_rows, std::vector<cell>(grid_cols));
          grid_dijkstra.resize(grid_rows, std::vector<cell>(grid_cols));

          // Initialize each cell with its grid coordinates
          for (int y = 0; y < grid_rows; y++) {
               for (int x = 0; x < grid_cols; x++) {
                    grid_a_star[y][x] = cell(x, y);
                    grid_dijkstra[y][x] = cell(x, y);
               }
          }
     }

     // Update game state and handle user interactions
     void update()
     {
          Vector2 mouse_pos = GetMousePosition();
          int grid_x = static_cast<int>(mouse_pos.x) / cell_size;
          int grid_y = static_cast<int>(mouse_pos.y) / cell_size;
          bool is_left_side = mouse_pos.x < static_cast<float>(screen_width) / 2;

          if (grid_x >= 0 && grid_x < grid_cols && grid_y >= 0 && grid_y < grid_rows)
          {
               auto& current_grid = is_left_side ? grid_a_star : grid_dijkstra;
               grid_x = is_left_side ? grid_x : grid_x - grid_cols;

               // Handle mouse left button interactions
               if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
               {
                    cell& current_cell = current_grid[grid_y][grid_x];
                    cell& mirror_cell = is_left_side ? grid_dijkstra[grid_y][grid_x] : grid_a_star[grid_y][grid_x];

                    // Place start cell
                    if (!start_cell_a_star && current_cell.type != END)
                    {
                         current_cell.type = START;
                         mirror_cell.type = START;
                         start_cell_a_star = &grid_a_star[grid_y][grid_x];
                         start_cell_dijkstra = &grid_dijkstra[grid_y][grid_x];
                    }
                    // Place end cell
                    else if (!end_cell_a_star && current_cell.type != START)
                    {
                         current_cell.type = END;
                         mirror_cell.type = END;
                         end_cell_a_star = &grid_a_star[grid_y][grid_x];
                         end_cell_dijkstra = &grid_dijkstra[grid_y][grid_x];
                    }
                    // Start placing walls
                    else {
                         is_placing_walls = true;
                    }
               }

               // Continue placing walls while mouse is held down
               if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && is_placing_walls)
               {
                    if (grid_x >= 0 && grid_x < grid_cols) {
                         if (current_grid[grid_y][grid_x].type != START &&
                              current_grid[grid_y][grid_x].type != END) {
                              current_grid[grid_y][grid_x].type = WALL;
                              auto& mirror_grid = is_left_side ? grid_dijkstra : grid_a_star;
                              mirror_grid[grid_y][grid_x].type = WALL;
                         }
                    }
               }
          }

          // Stop placing walls
          if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
          {
               is_placing_walls = false;
          }

          // Trigger pathfinding when spacebar is pressed
          if (IsKeyPressed(KEY_SPACE))
          {
               a_star_stats = find_path(grid_a_star, start_cell_a_star, end_cell_a_star, true);
               dijkstra_stats = find_path(grid_dijkstra, start_cell_dijkstra, end_cell_dijkstra, false);
          }

          // Reset grid when 'R' is pressed
          if (IsKeyPressed(KEY_R))
          {
               for (int y = 0; y < grid_rows; y++)
               {
                    for (int x = 0; x < grid_cols; x++)
                    {
                         grid_a_star[y][x] = cell(x, y);
                         grid_dijkstra[y][x] = cell(x, y);
                    }
               }
               start_cell_a_star = start_cell_dijkstra = nullptr;
               end_cell_a_star = end_cell_dijkstra = nullptr;
               a_star_stats = path_stats();
               dijkstra_stats = path_stats();
          }
     }

     /// Render the visualization
     void draw() {
          ClearBackground(RAYWHITE);

          // Draw vertical separator
          DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

          // Draw grid rendering lambda
          auto draw_grid = [this](const std::vector<std::vector<cell>>& grid, int offset_x)
               {
                    for (int y = 0; y < grid_rows; y++) {
                         for (int x = 0; x < grid_cols; x++) {
                              Rectangle cell_rect = {
                                  static_cast<float>(x * cell_size + offset_x),
                                  static_cast<float>(y * cell_size),
                                  static_cast<float>(cell_size),
                                  static_cast<float>(cell_size)
                              };

                              // Color selection based on cell

                              Color cell_color;
                              switch (grid[y][x].type)
                              {
                              case WALL:
                                   cell_color = { 75, 75, 75, 255 };
                                   break;
                              case START:
                                   cell_color = GREEN;
                                   break;
                              case END:
                                   cell_color = RED;
                                   break;
                              case PATH:
                                   cell_color = YELLOW;
                                   break;
                              case VISITED:
                                   cell_color = get_visited_color(grid[y][x].visit_order);
                                   break;
                              default:
                                   cell_color = BLACK;
                                   break;
                              }

                              // Draw cell with optional grid lines
                              DrawRectangleRec(cell_rect, cell_color);
                              DrawRectangleLinesEx(cell_rect, 1, { 127, 127, 127, 50 });
                         }
                    }
               };

          // Draw both grids
          draw_grid(grid_a_star, 0);
          draw_grid(grid_dijkstra, screen_width / 2);

          // Draw A* algorithm statistics
          DrawText("A* Algorithm", 10, 10, 20, SKYBLUE);
          DrawText(("Time: " + std::to_string(a_star_stats.execution_time) + " ms").c_str(),
               10, 35, 20, SKYBLUE);
          DrawText(("Nodes visited: " + std::to_string(a_star_stats.nodes_visited)).c_str(),
               10, 60, 20, SKYBLUE);
          DrawText(("Path length: " + std::to_string(a_star_stats.path_length)).c_str(),
               10, 85, 20, SKYBLUE);

          // Vertical partition
          DrawRectangle(screen_width / 2, 0, 5, screen_width, BLACK);

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
};


int main()
{

     InitWindow(screen_width, screen_height, "Pathfinding");


     pathfinding_visualizer visualizer;


     while (!WindowShouldClose()) {

          visualizer.update();


          BeginDrawing();
          visualizer.draw();


          EndDrawing();
     }


     CloseWindow();
     return 0;
}