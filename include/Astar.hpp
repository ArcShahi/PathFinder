#pragma once

#include <IPathfinder.hpp>
#include <unordered_map>
#include <queue>

class AStar : public IPathFinder {
public:
	void Init(const Grid& grid, Vector2 start, Vector2 end) override;
	bool Step() override;
	const std::vector<Vector2>& GetVisited() const override { return m_Visited; }
	const std::vector<Vector2>& GetPath() const override { return m_Path; }
	bool Found() const override { return m_Found; }

private:
	struct Node
	{
		int x{}, y{};
		float f{};
		bool operator>(const Node& o) const { return f > o.f; }
	};

	static int Key(int x, int y, int cols) { return y * cols + x; }
	float Heuristic(int x, int y) const;
	void ReconstructPath();

	const Grid* m_Grid{ nullptr };
	Vector2 m_Start{}, m_End{};

	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> m_Open{};
	std::unordered_map<int, float> m_GScore{};
	std::unordered_map<int, int> m_CameFrom{};   // key -> key
	std::vector<bool> m_Closed{};              // may cause some bug

	std::vector<Vector2> m_Visited{};
	std::vector<Vector2> m_Path{};
	bool m_Found{ false };
	bool m_Done{ false };
};