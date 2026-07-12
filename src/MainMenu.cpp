#include <MainMenu.hpp>
#include <imgui.h>
#include <algorithm>
#include <execution>

MainMenu::MainMenu()
{
	
}

void MainMenu::Update()
{
	for (auto& star : m_Stars)
		star.Update(GetFrameTime());
	

	if (IsKeyPressed(KEY_A))
	{
		m_RunningAlgo = Algorithms::AStar;
		m_NextState = StateID::Pathfinder;
	}

	if (IsKeyPressed(KEY_D))
	{
		m_RunningAlgo = Algorithms::Dijkstra;
		m_NextState = StateID::Pathfinder;
	}

	if (IsKeyPressed(KEY_V))
	{
		m_NextState = StateID::Versus;
	}
}

void MainMenu::Render()
{
	for (auto& stars : m_Stars)
		stars.Render();
	const int height = GetScreenHeight();

	DrawCenteredText("PathFinder Sandbox", height * 0.20f, 50, PINK);
	DrawCenteredText("[A] A*", height * 0.45f, 25, WHITE);
	DrawCenteredText("[D] Dijkstra", height * 0.52f, 25, WHITE);
	DrawCenteredText("[V] Versus", height * 0.59f, 25, WHITE);
	DrawCenteredText("[ESC] Exit", height * 0.66f, 25, WHITE);
	DrawText("@Shahi", GetScreenWidth()*0.94f, height*0.95f, 15, RED);
}

void MainMenu::DrawCenteredText(const char* text, int y, int FontSize, Color color)
{
	int textWidth = MeasureText(text, FontSize);

	DrawText(
		text,
		(GetScreenWidth() - textWidth) / 2,
		y,
		FontSize,
		color
	);
}
