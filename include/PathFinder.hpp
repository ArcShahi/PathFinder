#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

class PathFinder
{
public:
	virtual ~PathFinder()=default;
	virtual void Start() = 0;
	virtual bool Step() = 0;
	virtual void Reset() = 0;
	virtual bool IsRunning()const = 0;
	virtual bool IsFinished() const= 0;

};


#endif // PATHFINDER_HPP