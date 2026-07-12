#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include "cell.hpp"


class Grid
{
public:
	Grid(int width, int height)
		:m_Width{ width }, m_Height{ height }, m_Cells{ width * height,CellType::EMPTY }
	{}

	CellType& At(int x, int y) { return m_Cells[y * m_Width + x]; }
	const CellType& At(int x, int y) const { return m_Cells[y * m_Width + x]; }

	int GetWidth()const { return m_Width; };
	int GetHeight()const { return m_Height; }

private:
	int m_Width{};
	int m_Height{};
	std::vector<CellType> m_Cells{};
};



#endif // !GRID_HPP