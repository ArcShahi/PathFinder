#pragma once

#include <State.hpp>

class MenuState : public State {
public:
	explicit MenuState(StateMachine& machine) : State(machine) {}
	void HandleInput() override;
	void Update(float dt) override {}
	void Draw() override;

private:
private:
	int m_Selected{ 0 }; // 0 = A*, 1 = Dijkstra, 2 = Versus
};

