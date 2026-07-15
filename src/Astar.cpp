#include <Astar.hpp>

void AStar::Init(const Grid& grid, Vector2 start, Vector2 end)
{
	m_Grid = &grid;
	m_Start = start;
	m_End = end;
	m_Found = false;
	m_Done = false;
	m_Visited.clear();
	m_Path.clear();
	m_GScore.clear();
	m_CameFrom.clear();
	m_Closed.assign(grid.Cols() * grid.Rows(), false);

	while (!m_Open.empty())
	{
		m_Open.pop();
	}

	int startKey{ Key(static_cast<int>(start.x),
		       static_cast<int>(start.y),
		       grid.Cols()) };

	m_GScore[startKey] = 0.0f;
	m_Open.push({ static_cast<int>(start.x),static_cast<int>(start.y),Heuristic(start.x,start.y) });

}

bool AStar::Step()
{
	if (m_Done) return true;
	if (m_Open.empty()) { m_Done = true; m_Found = false; return true; }

	Node current{ m_Open.top() };
	m_Open.pop();

	int cols{ m_Grid->Cols() }, Rows{ m_Grid->Rows() };
	int curKey{ Key(current.x,current.y,cols) };

	if (m_Closed[curKey]) return false;  // State entry
	m_Closed[curKey] = true;
	m_Visited.push_back({ static_cast<float>(current.x),
		               static_cast<float>(current.y) });

	if (current.x == static_cast<int>(m_End.x) && current.y == static_cast<int>(m_End.y))
	{
		m_Found = true;
		m_Done = true;
		ReconstructPath();
		true;
	}
	
	static const int dx[4] { 1,-1,0,0 };
	static const int dy[4] { 0,0,1,-1 };

	for (int i{ 0 }; i < 4; ++i)
	{
		int nx{ current.x + dx[i] }, ny{ current.y + dy[i] };
		if (nx < 0 || nx >= cols || ny < 0 || ny >= Rows)continue;
		if (m_Grid->IsWall(nx, ny)) continue;

		int neighborKey = Key(nx, ny, cols);
		if (m_Closed[neighborKey])continue;

		float tenativeG{ m_GScore[curKey] + 1.0f };
		auto it{ m_GScore.find(neighborKey) };

		if (it == m_GScore.end() || tenativeG < it->second)
		{
			m_GScore[neighborKey] = tenativeG;
			m_CameFrom[neighborKey] = curKey;

			float f{ tenativeG +Heuristic(nx,ny)};
			m_Open.push({ nx,ny,f });
		}
	}
	return false;
}

float AStar::Heuristic(int x, int y) const
{
	// Manhattan distance for now , will improve later
	return std::abs(x - m_End.x) + std::abs(y - m_End.y); 
}

void AStar::ReconstructPath()
{
	int cols{ m_Grid->Cols() };
	int key{ Key(static_cast<int>(m_End.x),static_cast<int>(m_End.y),cols) };
	int startKey{ Key(static_cast<int>(m_Start.x),static_cast<int>(m_Start.y),cols) };

	while (key!=startKey)
	{
		m_Path.push_back({ static_cast<float>(key % cols),static_cast<float>(key / cols) });
		key = m_CameFrom[key];
	}
	m_Path.push_back(m_Start);
}
