#pragma once

#include <State.hpp>
#include <string>

class VisualizeState:public State
{
public:
	VisualizeState(StateMachine& machine, std::string label)
		:State{machine}, m_Label{ label }
	{}

	void HandleInput() override;
	void Update(float dt) override;
	void Draw()override;

private:
	std::string m_Label{};
};

