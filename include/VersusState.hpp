#pragma once
#include <State.hpp>


class VersusState : public State {
public:
	explicit VersusState(StateMachine& machine) : State(machine) {}
	void HandleInput() override;
	void Update(float dt) override {}
	void Draw() override;


};