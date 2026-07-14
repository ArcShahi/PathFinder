#include <UI.hpp>
#include <imgui.h>

static void ColorEditRGB(const char* label, Color& c) {
     float f[3] = { c.r / 255.0f, c.g / 255.0f, c.b / 255.0f };
     if (ImGui::ColorEdit3(label, f)) {
          c.r = (unsigned char)(f[0] * 255);
          c.g = (unsigned char)(f[1] * 255);
          c.b = (unsigned char)(f[2] * 255);
     }
}

static void DrawStatsBlock(const char* label, VizStats& stats, GridColors& colors) {
     ImGui::PushID(label);
     ImGui::Text("%s", label);
     ColorEditRGB("Visited 0", colors.visitedStart);
     ColorEditRGB("Visited 1", colors.visitedEnd);
     ColorEditRGB("Path", colors.path);

     ImGui::Text("Time: %.2fs", stats.elapsedTime);
     ImGui::Text("Nodes visited: %d", stats.nodesVisited);
     ImGui::Text("Path length: %d", stats.pathLength);
     if (stats.finished)
          ImGui::Text(stats.found ? "Path found!" : "No path exists.");

     ImGui::PopID();
}


void DrawVizPanel(const char* label, VizSettings& settings, VizStats& stats, GridColors& colors)
{
     ImGui::Begin(label);

     if (ImGui::Button(settings.paused ? "Resume" : "Pause"))
          settings.paused = !settings.paused;

     ImGui::SameLine();
     if (ImGui::Button("Restart")) settings.restartRequested = true;
     ImGui::SameLine();
     if (ImGui::Button("Reset")) settings.resetRequested = true;
 
     ImGui::SliderFloat("Speed", &settings.steps, 1.0f,500.f,"%.0f steps/sec");

     ImGui::Separator();
     ColorEditRGB("Grid", colors.empty);
     ColorEditRGB("Grid Edges", colors.gridLine);
     ColorEditRGB("Visited (start)", colors.visitedStart);
     ColorEditRGB("Visited (end)", colors.visitedEnd);
     ColorEditRGB("Path", colors.path);
     ColorEditRGB("Wall", colors.wall);
     ColorEditRGB("Start", colors.start);
     ColorEditRGB("End", colors.end);

     ImGui::Separator();
     ImGui::Text("Time: %.2fs", stats.elapsedTime);
     ImGui::Text("Nodes visited: %d", stats.nodesVisited);
     ImGui::Text("Path length: %d", stats.pathLength);
     if (stats.finished)
          ImGui::Text(stats.found ? "Path found!" : "No path exists.");


     ImGui::End();
}

void DrawVizPanel(VizSettings& settings, VizStats& statsA, GridColors& colorsA, VizStats& statsB, GridColors& colorsB)
{
     ImGui::Begin("Versus Controls");

     if (ImGui::Button(settings.paused ? "Resume" : "Pause")) settings.paused = !settings.paused;
     ImGui::SameLine();
     if (ImGui::Button("Restart")) settings.restartRequested = true;
     ImGui::SameLine();
     if (ImGui::Button("Reset")) settings.resetRequested = true;

     ImGui::SliderFloat("Speed", &settings.steps, 1.0f, 500.f, "%.0f steps/sec");

     ImGui::Separator();
     ColorEditRGB("Grid", colorsA.empty);
     ColorEditRGB("Wall", colorsA.wall);
     ColorEditRGB("Start", colorsA.start);
     ColorEditRGB("End", colorsA.end);
     colorsB.empty = colorsA.empty;
     colorsB.wall = colorsA.wall;
     colorsB.start = colorsA.start;
     colorsB.end = colorsA.end;

     ImGui::Separator();
     DrawStatsBlock("A*", statsA, colorsA);
     ImGui::NextColumn();
     DrawStatsBlock("Dijkstra", statsB, colorsB);
     ImGui::Columns(1);

     ImGui::End();
}
