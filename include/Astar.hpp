#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <pathfinder.hpp>
#include <grid.hpp>

class Astar :public PathFinder
{
public:
	explicit Astar(Grid& grid);
	void Start()override;
	bool Step()override;
	void Reset() override;
	bool IsRunning()const override;
	bool IsFinished()const override;
private:

	Grid& m_Grid;
};


#endif // !ASTAR_HPP
