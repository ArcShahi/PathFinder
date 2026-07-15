#include <Grid.hpp>

Color LerpColor(Color a, Color b, float t)
{
	return {
		static_cast<uint8_t>(a.r + (b.r - a.r) * t),
		static_cast<uint8_t>(a.g + (b.g - a.g) * t),
		static_cast<uint8_t>(a.b + (b.b - a.b) * t),
		static_cast<uint8_t>(a.a + (b.a - a.a) * t)
	};
}


Grid::Grid(int cols, int rows)
	: m_Cells(cols* rows, CellType::Empty) 
{
	ComputeGridDimension(cols, rows);
}


float Grid::CellSize(Rectangle& bound) const
{
	return std::min(bound.width / m_Cols, bound.height / m_Rows);
}

Vector2 Grid::ScreenToCell(Vector2 mouse, Rectangle& bounds)const
{
	float cell{ CellSize(bounds) };

	int cx{ static_cast<int>((mouse.x - bounds.x) / cell) };
	int cy{ static_cast<int>((mouse.y - bounds.y) / cell) };

	if (cx < 0 || cx >= m_Cols || cy < 0 || cy >= m_Rows) { return { -1,-1 }; }
	return { static_cast<float>(cx),static_cast<float>(cy) };

}

void Grid::Draw(Rectangle bounds, const GridColors& colors,
	const std::vector<Vector2>* visited,
	const std::vector<Vector2>* path) const {
	float cell = CellSize(bounds);

	for (int y = 0; y < m_Rows; ++y) {
		for (int x = 0; x < m_Cols; ++x) {
			Rectangle r = { bounds.x + x * cell, bounds.y + y * cell, cell, cell };
			Color c = IsWall(x, y) ? colors.wall : colors.empty;
			DrawRectangleRec(r, c);
			DrawRectangleLinesEx(r, 1.0f, colors.gridLine);
		}
	}

	if (visited && !visited->empty()) 
	{
		size_t n = visited->size();
		for (size_t i = 0; i < n; ++i) {
			float t = (n > 1) ? (float)i / (float)(n - 1) : 0.0f;
			Color c = LerpColor(colors.visitedStart, colors.visitedEnd, t);
			const Vector2& v = (*visited)[i];
			Rectangle r = { bounds.x + v.x * cell, bounds.y + v.y * cell, cell, cell };
			DrawRectangleRec(r, c);
		}
	}

	if (path) {
		for (const auto& p : *path) {
			Rectangle r = { bounds.x + p.x * cell, bounds.y + p.y * cell, cell, cell };
			DrawRectangleRec(r, colors.path);
		}
	}

	Rectangle sr  { bounds.x + m_Start.x * cell, bounds.y + m_Start.y * cell, cell, cell };
	Rectangle er  { bounds.x + m_End.x * cell,   bounds.y + m_End.y * cell,   cell, cell };
	DrawRectangleRec(sr, colors.start);
	DrawRectangleRec(er, colors.end);
}



void Grid::SetWall(int x, int y, bool isWall)
{
	if (static_cast<int>(m_Start.x) == x && static_cast<int>(m_Start.y) == y) return;
	if (static_cast<int>(m_End.x) == x && static_cast<int>(m_End.y) == y) return;
	m_Cells[y * m_Cols + x] = isWall ? CellType::Wall : CellType::Empty;
}

bool Grid::IsWall(int x, int y) const
{
	return m_Cells[y * m_Cols + x] == CellType::Wall;
}

void Grid::ClearWalls()
{
	std::fill(m_Cells.begin(), m_Cells.end(), CellType::Empty);
}

void Grid::ComputeGridDimension(int width, int height)
{
	m_Cols = std::max(1, (width / m_Size));
	m_Rows = std::max(1, (height / m_Size));
	
}

