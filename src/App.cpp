#include "raylib.h"
#include <StateMachine.hpp>
#include <MenuState.hpp>
#include <VersusState.hpp>
#include <VisualizeState.hpp>


int main() {
     InitWindow(1000, 700, "PathFinder");
     SetTargetFPS(60);

     StateMachine machine;
     machine.RegisterFactory(StateID::MainMenu, [&machine] {
          return std::make_unique<MenuState>(machine);
          });
     machine.RegisterFactory(StateID::AStar, [&machine] {
          return std::make_unique<VisualizeState>(machine, "A*");
          });
     machine.RegisterFactory(StateID::Dijkstra, [&machine] {
          return std::make_unique<VisualizeState>(machine, "Dijkstra");
          });
     machine.RegisterFactory(StateID::Versus, [&machine] {
          return std::make_unique<VersusState>(machine);
          });

     machine.RequestChange(StateID::MainMenu);
     machine.ApplyPendingChange();

     while (!WindowShouldClose()) {
          float dt = GetFrameTime();
          machine.HandleInput();
          machine.Update(dt);
          machine.ApplyPendingChange();

          BeginDrawing();
          ClearBackground(BLACK);
          machine.Draw();
          EndDrawing();
     }

     CloseWindow();
     return 0;
}