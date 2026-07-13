#include <VisualizeState.hpp>
#include <raylib.h>


void VisualizeState::HandleInput()
{
	if (IsKeyPressed(KEY_BACKSPACE)) RequestChange(StateID::MainMenu);
}

void VisualizeState::Update(float dt)
{}

void VisualizeState::Draw() {
	DrawText(TextFormat("Visualizing: %s", m_Label.c_str()), 100, 100, 30, WHITE);
	DrawText("BACKSPACE to go back", 100, 150, 18, GRAY);
}