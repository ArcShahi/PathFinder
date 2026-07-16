#pragma once

#include <Grid.hpp> // for GridColors
#include <raylib.h>

struct VizSettings {
  float steps{100.0f};
  bool paused{false};
  bool resetRequested{false};
  bool rerunRequested{false};
};

struct VizStats {
  float elapsedTime{0.0f};
  int nodesVisited{0};
  int pathLength{0};
  bool finished{false};
  bool found{false};
  bool help{false};
};

void DrawVizPanel(const char *label, VizSettings &settings, VizStats &stats,
                  GridColors &colors);
void DrawVizPanel(VizSettings &settings, VizStats &statsA, GridColors &colorsA,
                  VizStats &statsB, GridColors &colorsB);
