#pragma once

#include <raylib.h>
#include <Grid.hpp> // for GridColors

struct VizSettings {
	bool paused = false;
	float stepInterval = 0.02f;
	bool resetRequested = false;
	bool restartRequested = false;
};

struct VizStats {
	float elapsedTime = 0.0f;
	int nodesVisited = 0;
	int pathLength = 0;
	bool finished = false;
	bool found = false;
};

void DrawVizPanel(const char* label, VizSettings& settings, VizStats& stats, GridColors& colors);