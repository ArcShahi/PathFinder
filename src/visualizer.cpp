#include "visualizer.hpp"
#include <raylib.h>

namespace na 
{
     visualizer::visualizer() 
     {
         
          InitWindow(screen_width, screen_height,"PathFinder");
          SetTargetFPS(144);
     }

     visualizer::~visualizer()
     {
          CloseWindow();
     }

     void visualizer::run() 
     {
          while (!WindowShouldClose())
          {
  
               update();
               render();
          }
     }

     void visualizer::update() 
     {
          path_finder.update();
     }

     void visualizer::render() 
     {
          BeginDrawing();
          ClearBackground(RAYWHITE);
         
         
          path_finder.render();
           
          DrawFPS(900, 10);
          EndDrawing();
     }
}