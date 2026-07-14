#include <MenuState.hpp>
#include <raylib.h>


static void DrawCenteredText(const char* text, int y, int FontSize, Color color)
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

void MenuState::HandleInput()
{
     if (IsKeyPressed(KEY_DOWN)) m_Selected = (m_Selected + 1) % 3;
     if (IsKeyPressed(KEY_UP))   m_Selected = (m_Selected + 2) % 3;

     if (IsKeyPressed(KEY_ENTER)) {
          switch (m_Selected) {
          case 0: RequestChange(StateID::AStar);    break;
          case 1: RequestChange(StateID::Dijkstra); break;
          case 2: RequestChange(StateID::Versus);   break;
          }
     }
}


void MenuState::Draw() {
     const char* options[3] = { "A*", "Dijkstra", "Versus" };
     const int height = GetScreenHeight();
     DrawCenteredText("PathFinder Sandbox", height * 0.20f, 50, PINK);
     float alignY{ 0.45f };

    
     for (int i = 0; i < 3; ++i) {
          Color c = (i == m_Selected) ? YELLOW : WHITE;
          //DrawText(options[i], 100, 100 + i * 40, 30, c);
          DrawCenteredText(options[i], height *alignY, 30, c);
          alignY += 0.07f;
     }
     DrawCenteredText("UP/DOWN to select, ENTER to confirm", height * alignY, 18, GRAY);
}


