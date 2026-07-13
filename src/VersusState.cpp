#include <VersusState.hpp>
#include <raylib.h>


void VersusState::HandleInput()
{
	if (IsKeyPressed(KEY_BACKSPACE)) RequestChange(StateID::MainMenu);
}

void VersusState::Draw() {
	DrawText("Versus Mode", 100, 100, 30, WHITE);
	DrawText("BACKSPACE to go back", 100, 150, 18, GRAY);
}