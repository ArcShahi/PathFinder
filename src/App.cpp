#include "App.hpp"
#include <MainMenu.hpp>
#include <rlImGui.h>

App::App()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "PathFinder");
	SetTargetFPS(144);

	rlImGuiSetup(true);

	m_CurrentState = std::make_unique<MainMenu>();
	m_CurrentState->OnEnter();
}

App::~App()
{
	rlImGuiShutdown();
	CloseWindow();
}

void App::Run()
{
	while (!WindowShouldClose())
	{

		Update();
		Render();
	}
}

void App::Update()
{
	m_CurrentState->Update();
}

void App::Render()
{
	BeginDrawing();
	ClearBackground(BLACK);


	m_CurrentState->Render();



	EndDrawing();
}
