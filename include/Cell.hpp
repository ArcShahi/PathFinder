#ifndef CELL_HPP
#define CELL_HPP

#include <vector>
#include <limits>



enum class CellType
{
	EMPTY,
	WALL,
	START,
	END,
	PATH,
	VISITED
};

constexpr float INF = std::numeric_limits<float>::infinity();

struct Cell
{
	Cell() = default;
	Cell(int x, int y) :x{ x }, y{ y } {}

	int x{};
	int y{};
	CellType type{ CellType::EMPTY };
	float Gcost{ INF };
	float Hcost{ 0 };            // for A* 
	float Fcost{ INF };
	Cell* Parent{ nullptr };
	int VisitOrder{ 0 };

	float GetFCost()const { return Gcost + Hcost; }
};

struct PathStats
{
	float execution_time{};
	int nodes_visited{};
	int path_length{};
	std::vector<Cell*> path{};
};


#endif // CELL_HPP