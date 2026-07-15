#pragma once
#include <State.hpp>
#include <Grid.hpp>
#include <Astar.hpp>
#include <Dijkstra.hpp>
#include <UI.hpp>
#include <memory>


class Versus : public State {
public:
	explicit Versus(StateMachine& machine);
	void HandleInput() override;
	void Update(float dt) override;
	void Draw() override;

private:
	Rectangle LeftBounds()const;
	Rectangle RightBounds()const;

	void PlaceOrToggle();
	void Reset();
	void Restart();
private:

	// One shared maze,we're gonna share it
	Grid m_Grid{};
	AStar m_Astar{};
	Dijkstra m_Dijkstra{};
	std::unique_ptr<IPathFinder>m_Versus{};
	GridColors m_ColorsA{}, m_ColorsB{};
	VizSettings m_Settings{};
	VizStats m_StatsA{}, m_StatsB{};

	float m_Accumulator{ 0.0f };
	bool m_Started{ false };
	bool m_PaintValue{ false };

};