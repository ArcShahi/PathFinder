#include <State.hpp>
#include <StateMachine.hpp>

void State::RequestChange(StateID id) { m_Machine.RequestChange(id); }
