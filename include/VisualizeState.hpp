#pragma once

#include <State.hpp>
#include <string>
#include <Grid.hpp>
#include <IPathfinder.hpp>
#include <UI.hpp>
#include <memory>


enum class Algo
{
	Astar,
	Dijkstra
};

class VisualizeState:public State
{
public:
	VisualizeState(StateMachine& machine, Algo algo);

	void HandleInput() override;
	void Update(float dt) override;
	void Draw()override;

private:
	void Reset();
	void Restart();
	void PlaceOrToggle(); //for shared mouse-click

private:
	Algo m_Algo{};
	Grid m_Grid{};
	std::unique_ptr<IPathFinder>m_Solver{};
	GridColors m_Colors{};
	VizSettings m_Settings{};
	VizStats m_Stats{};

	float m_Accumulator{ 0.0f };
	bool m_Started{ false };
	bool m_PaintValue{ false };
};

