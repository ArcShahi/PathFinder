#include <raylib.h>
#include <rlImGui.h>
#include <StateMachine.hpp>
#include <MenuState.hpp>
#include <VersusState.hpp>
#include <VisualizeState.hpp>


int main() {

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "PathFinder");
	SetWindowMinSize(600, 400);
	rlImGuiSetup(true);

	StateMachine machine;
	machine.RegisterFactory(StateID::MainMenu, [&machine] { return std::make_unique<MenuState>(machine); });
	machine.RegisterFactory(StateID::AStar, [&machine] {
		return std::make_unique<VisualizeState>(machine, Algo::Astar);
		});
	machine.RegisterFactory(StateID::Dijkstra, [&machine] {
		return std::make_unique<VisualizeState>(machine, Algo::Dijkstra);
		});
	// ... Versus factory later

	machine.RequestChange(StateID::MainMenu);
	machine.ApplyPendingChange();

	while (!WindowShouldClose()) {
		float dt = GetFrameTime();
		if (IsKeyPressed(KEY_F11)) ToggleBorderlessWindowed();
		machine.HandleInput();
		machine.Update(dt);
		machine.ApplyPendingChange();

		BeginDrawing();
		ClearBackground(BLACK);
		machine.Draw();
		EndDrawing();
	}
	rlImGuiShutdown();
	CloseWindow();
}