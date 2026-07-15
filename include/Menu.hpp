#pragma once

#include <State.hpp>
#include <Star.hpp>
#include <vector>

class Menu : public State {
public:
	explicit Menu(StateMachine& machine) : State(machine) {}
	void HandleInput() override;
	void Update(float dt) override;
	void Draw() override;

private:
	std::vector<Star> m_Stars{ 500 };
	int m_Selected{ 0 }; // 0 = A*, 1 = Dijkstra, 2 = Versus
};

