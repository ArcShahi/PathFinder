#include "App.hpp"
#include <MainMenu.hpp>
#include <rlImGui.h>

App::App()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "PathFinder");
	SetTargetFPS(144);

	rlImGuiSetup(true);

	m_CurrentScene = std::make_unique<MainMenu>();
	m_CurrentScene->OnEnter();
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
	m_CurrentScene->Update();
}

void App::Render()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);
	rlImGuiBegin();


	m_CurrentScene->Render();



	rlImGuiEnd();
	EndDrawing();
}
