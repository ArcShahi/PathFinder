#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <raylib.h>
#include <vector>
#include "cell.hpp"


constexpr int screen_width = 1920;
constexpr int screen_height = 1000;
constexpr int cell_size = 15;          // can change  but will slow down performance[limit 1]
constexpr int grid_cols = (screen_width / 2) / cell_size;
constexpr int grid_rows = screen_height / cell_size;

namespace na
{
	class pathfinder
	{
	public:
		pathfinder();
		void update();
		void render();

	private:
		using grid = std::vector<std::vector<cell>>;



		grid a_star_grid;
		grid dijkstra_grid;

		cell* start_cell_a_star{};
		cell* end_cell_a_star{};
		cell* start_cell_dijkstra{};
		cell* end_cell_dijkstra{};

		bool is_placing_walls{ false };
		path_stats a_star_stats{};
		path_stats dijkstra_stats{};
		int total_visited_nodes{};

		float calculate_heuristic(const cell* current, const cell* end) const;
		std::vector<cell*> get_neighbors(cell* current, grid& grid);
		Color get_visited_color(int visit_order) const;
		path_stats find_path(grid& grid, cell* start, cell* end, bool use_a_star);


		float accum_time{ 0.0f };
		const float UPDATE_INTERVAL = 1.0f / 60.f;
	};
}

#endif // PATHFINDER_HPP