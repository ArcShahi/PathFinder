#include <UI.hpp>
#include <imgui.h>

static void ColorEditRGB(const char *label, Color &c) {
  float f[3]{c.r / 255.0f, c.g / 255.0f, c.b / 255.0f};
  if (ImGui::ColorEdit3(label, f)) {
    c.r = (unsigned char)(f[0] * 255);
    c.g = (unsigned char)(f[1] * 255);
    c.b = (unsigned char)(f[2] * 255);
  }
}

static void DrawStatsBlock(const char *label, VizStats &stats,
                           GridColors &colors) {
  ImGui::PushID(label);
  ImGui::Text("%s", label);
  ColorEditRGB("trail -", colors.visitedStart);
  ColorEditRGB("trail +", colors.visitedEnd);
  ColorEditRGB("Path", colors.path);

  ImGui::Text("Time: %.2fs", stats.elapsedTime);
  ImGui::Text("Nodes visited: %d", stats.nodesVisited);
  ImGui::Text("Path length: %d", stats.pathLength);
  if (stats.finished)
    ImGui::Text(stats.found ? "Path found!" : "No path exists.");

  ImGui::PopID();
}

void DrawVizPanel(const char *label, VizSettings &settings, VizStats &stats,
                  GridColors &colors) {
  ImGui::Begin(label);
  ImGui::Text("FPS %.1f", ImGui::GetIO().Framerate);

  ImGui::Separator();
  ColorEditRGB("grid", colors.empty);
  ColorEditRGB("grid lines", colors.gridLine);
  ColorEditRGB("trail -", colors.visitedStart);
  ColorEditRGB("trail +", colors.visitedEnd);
  ColorEditRGB("path", colors.path);
  ColorEditRGB("wall", colors.wall);
  ColorEditRGB("start", colors.start);
  ColorEditRGB("end", colors.end);

  ImGui::Separator();

  if (!settings.paused) {
    if (ImGui::Button("pause"))
      settings.paused = true;
  } else {
    ImGui::Text("Paused: can edit walls, then Rerun");
  }

  if (ImGui::Button("re-run"))
    settings.rerunRequested = true;
  ImGui::SameLine();
  if (ImGui::Button("reset"))
    settings.resetRequested = true;

  // Help Box
  ImGui::SameLine();
  if (ImGui::Button("help")) {
    stats.help = !stats.help;
  }
  if (stats.help) {
    ImGui::Text("SPACE to run\n"
                "Hold LMB and drag to place walls\n"
                "Hold RMB and drag to erase wall\n");
  }
  ImGui::Separator();
  ImGui::SliderFloat("speed", &settings.steps, 1.0f, 1000.f, "%.0f steps/sec");
  ImGui::Separator();
  ImGui::Text("Time: %.2fs", stats.elapsedTime);
  ImGui::Text("Nodes visited: %d", stats.nodesVisited);
  ImGui::Text("Path length: %d", stats.pathLength);
  if (stats.finished)
    ImGui::Text(stats.found ? "Path found!" : "No path exists.");

  ImGui::End();
}

void DrawVizPanel(VizSettings &settings, VizStats &statsA, GridColors &colorsA,
                  VizStats &statsB, GridColors &colorsB) {
  ImGui::Begin("Versus Controls");
  ImGui::Text("FPS %.1f", ImGui::GetIO().Framerate);

  ImGui::Separator();
  ColorEditRGB("grid", colorsA.empty);
  ColorEditRGB("grid lines", colorsA.gridLine);
  ColorEditRGB("wall", colorsA.wall);
  ColorEditRGB("start", colorsA.start);
  ColorEditRGB("end", colorsA.end);
  colorsB.empty = colorsA.empty;
  colorsB.wall = colorsA.wall;
  colorsB.gridLine = colorsA.gridLine;
  colorsB.start = colorsA.start;
  colorsB.end = colorsA.end;

  ImGui::Separator();
  DrawStatsBlock("A*", statsA, colorsA);
  ImGui::Separator();
  ImGui::Separator();
  DrawStatsBlock("Dijkstra", statsB, colorsB);

  ImGui::Separator();

  if (!settings.paused) {
    if (ImGui::Button("pause"))
      settings.paused = true;
  } else {
    ImGui::Text("Paused: can edit walls, then Rerun");
  }

  if (ImGui::Button("re-run"))
    settings.rerunRequested = true;
  ImGui::SameLine();
  if (ImGui::Button("reset"))
    settings.resetRequested = true;
  ImGui::Separator();

  ImGui::SliderFloat("speed", &settings.steps, 1.0f, 1000.f, "%.0f steps/sec");
  ImGui::End();
}
