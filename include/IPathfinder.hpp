#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP
#include <Grid.hpp>
#include <raylib.h>

class IPathFinder
{
public:
	virtual ~IPathFinder()=default;
	virtual void Init(const Grid& grid, Vector2 start, Vector2 end) = 0;
	virtual bool Step() = 0;

	virtual const std::vector<Vector2>& GetVisited()const = 0;
	virtual const std::vector<Vector2>& GetPath()const = 0;
	virtual bool Found()const = 0;
};


#endif // PATHFINDER_HPP