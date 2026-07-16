#include <Menu.hpp>
#include <Solver.hpp>
#include <StateMachine.hpp>
#include <Versus.hpp>
#include <raylib.h>
#include <rlImGui.h>

int main() {

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1280, 720, "Pathfinder Sandox");
  SetWindowMinSize(600, 400);

  rlImGuiSetup(true);

  StateMachine machine;
  machine.RegisterFactory(StateID::MainMenu, [&machine] {
    return std::make_unique<Menu>(machine);
  });
  machine.RegisterFactory(StateID::AStar, [&machine] {
    return std::make_unique<Solver>(machine, Algo::Astar);
  });
  machine.RegisterFactory(StateID::Dijkstra, [&machine] {
    return std::make_unique<Solver>(machine, Algo::Dijkstra);
  });
  machine.RegisterFactory(StateID::Versus, [&machine] {
    return std::make_unique<Versus>(machine);
  });

  machine.RequestChange(StateID::MainMenu);
  machine.ApplyPendingChange();

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    if (IsKeyPressed(KEY_F11))
      ToggleBorderlessWindowed();
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