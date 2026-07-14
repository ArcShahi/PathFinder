#include <VisualizeState.hpp>
#include <raylib.h>
#include <Astar.hpp>
#include <rlImGui.h>
#include <Dijkstra.hpp>
#include <imgui.h>


VisualizeState::VisualizeState(StateMachine& machine, Algo algo)
	: State(machine), m_Algo(algo), m_Grid(GetScreenWidth() / 25, GetScreenHeight() / 25)
{
	m_Solver = (algo == Algo::Astar) ?
		std::unique_ptr<IPathFinder>(std::make_unique<AStar>()) :
		std::unique_ptr<IPathFinder>(std::make_unique<Dijkstra>());

}

Rectangle GetBounds() {
	return { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
}

void VisualizeState::HandleInput()
{
	if (IsKeyPressed(KEY_BACKSPACE)) { RequestChange(StateID::MainMenu); return; }

	if (!m_Started)
	{
		PlaceOrToggle();
		if (IsKeyPressed(KEY_SPACE)) {
			m_Solver->Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
			m_Started = true;
		}
		return;
	}

	// once running, editing the maze mid-search doesn't make sense — ignore clicks
}
void VisualizeState::Update(float dt)
{
	if (!m_Started) return;
	if (m_Settings.paused || m_Stats.finished) return;

	m_Accumulator += dt;
	while (m_Accumulator >= m_Settings.stepInterval && !m_Stats.finished) {
		m_Stats.finished = m_Solver->Step();
		m_Accumulator -= m_Settings.stepInterval;
	}

	m_Stats.elapsedTime += dt;
	m_Stats.nodesVisited = (int)m_Solver->GetVisited().size();
	if (m_Stats.finished) {
		m_Stats.found = m_Solver->Found();
		m_Stats.pathLength = (int)m_Solver->GetPath().size();
	}
}

void VisualizeState::Draw() {

	Rectangle bounds = GetBounds();

	const auto* visited = m_Started ? &m_Solver->GetVisited() : nullptr;
	const auto* path = (m_Started && m_Stats.finished && m_Stats.found) ?
		&m_Solver->GetPath() : nullptr;
	m_Grid.Draw(bounds, m_Colors, visited, path);

	if (!m_Started) {
		DrawText("Left-click drag: Wall | Right-click drag: Erase| S+right-click: start | E+right-click: end | SPACE: run",
			10, GetScreenHeight() - 30, 16, BLACK);
	}

	rlImGuiBegin();
	const char* label = (m_Algo == Algo::Astar) ? "A* Controls" : "Dijkstra Controls";
	DrawVizPanel(label, m_Settings, m_Stats, m_Colors);
	rlImGuiEnd();

	if (m_Settings.resetRequested) {
		Reset();
		m_Settings.resetRequested = false;
	}


	if (m_Settings.restartRequested)
	{
		Restart();
		m_Settings.restartRequested = false;
	}
}

void VisualizeState::Reset()
{
	m_Grid.ClearWalls();
	m_Solver->Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
	m_Stats = VizStats{};
	m_Accumulator = 0.0f;
	m_Started = false; // enbale edit

	return;
}

void VisualizeState::Restart()
{
	m_Solver->Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
	m_Stats = VizStats{};
	m_Accumulator = 0.0f;
	m_Settings.paused = false; // so it doesn't restart into a paused state

}

void VisualizeState::PlaceOrToggle() {

	if (ImGui::GetIO().WantCaptureMouse) return;

	Rectangle bounds{ GetBounds() };
	Vector2 cell = m_Grid.ScreenToCell(GetMousePosition(), bounds);
	if (cell.x < 0) return;
	int cx = (int)cell.x, cy = (int)cell.y;

	bool placingStart{ IsKeyDown(KEY_S) };
	bool placingEnd{ IsKeyDown(KEY_E) };

	if (placingStart && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		m_Grid.SetStart(cx, cy);
		return;
	}
	if (placingEnd && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		m_Grid.SetEnd(cx, cy);
		return;
	}

	if (placingStart || placingEnd) return;
	// held (not just pressed) so dragging paints continuously
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		m_PaintValue = !m_Grid.IsWall(cx, cy);
		m_Grid.SetWall(cx, cy, m_PaintValue);
	}
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))       m_Grid.SetWall(cx, cy, m_PaintValue);
	else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) m_Grid.SetWall(cx, cy, m_PaintValue);
}