#pragma once

#include <memory>


enum class StateID
{
	MainMenu,
	AStar,
	Dijkstra,
	Versus,
	None
};

class StateMachine; // fwd declaration only, for preventing ciculation dependency

class State
{
public:
	using Ptr = std::unique_ptr<State>;
	explicit State(StateMachine& machine)
		:m_Machine{ machine }
	{}
	virtual ~State() = default;
	virtual void HandleInput() = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;

protected:
	void RequestChange(StateID id);
private:
	StateMachine& m_Machine;
};
