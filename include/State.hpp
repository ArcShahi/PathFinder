#ifndef SCENE_HPP
#define SCENE_HPP

enum class StateID
{
	None,
	MainMenu,
	Pathfinder,
	Versus,
	Exit
};

enum class  Algorithms
{
	AStar,
	Dijkstra,
	Versus
};

class State
{
public:
	virtual ~State() = default;

	virtual void OnEnter() {}
	virtual void OnExit() {}

	virtual void Update() = 0;
	virtual void Render() = 0;

	StateID NextState()const { return m_NextState; }
	Algorithms Algorithm()const { return m_RunningAlgo; }

protected:
	StateID m_NextState{ StateID::None };
	Algorithms m_RunningAlgo{ Algorithms::AStar };
};
#endif // !SCENE_HPP
