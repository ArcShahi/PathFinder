#include <Astar.hpp>
#include <Dijkstra.hpp>
#include <Solver.hpp>
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

Solver::Solver(StateMachine &machine, Algo algo)
    : State(machine), m_Algo(algo),
      m_Grid(GetScreenWidth(), GetScreenHeight()) {

  m_Grid.SetStart(0, 0);
  m_Grid.SetEnd(m_Grid.Cols() - 1, m_Grid.Rows() - 1);

  m_Solver = (algo == Algo::Astar)
                 ? std::unique_ptr<IPathFinder>(std::make_unique<AStar>())
                 : std::unique_ptr<IPathFinder>(std::make_unique<Dijkstra>());
}

Rectangle GetBounds() {
  return {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()};
}

void Solver::HandleInput() {
  if (IsKeyPressed(KEY_BACKSPACE)) {
    RequestChange(StateID::MainMenu);
    return;
  }

  if (!m_Started || m_Settings.paused) {
    PlaceOrToggle();
    if (!m_Started &&IsKeyPressed(KEY_SPACE)) {
      m_Solver->Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
      m_Started = true;
    }
    return;
  }
}
void Solver::Update(float dt) {
  if (!m_Started)
    return;
  if (m_Settings.paused || m_Stats.finished)
    return;

  float stepInterval{1.0f / m_Settings.steps};
  m_Accumulator += dt;
  while (m_Accumulator >= stepInterval && !m_Stats.finished) {
    m_Stats.finished = m_Solver->Step();
    m_Accumulator -= stepInterval;
  }

  m_Stats.elapsedTime += dt;
  m_Stats.nodesVisited = static_cast<int>(m_Solver->GetVisited().size());

  if (m_Stats.finished) {
    m_Stats.found = m_Solver->Found();
    m_Stats.pathLength = static_cast<int>(m_Solver->GetPath().size());
  }
}

void Solver::Draw() {

  Rectangle bounds{GetBounds()};

  const auto *visited{m_Started ? &m_Solver->GetVisited() : nullptr};
  const auto *path{(m_Started && m_Stats.finished && m_Stats.found)
                       ? &m_Solver->GetPath()
                       : nullptr};
  m_Grid.Draw(bounds, m_Colors, visited, path);

  rlImGuiBegin();
  const char *label{(m_Algo == Algo::Astar) ? "A* Controls"
                                            : "Dijkstra Controls"};

  DrawVizPanel(label, m_Settings, m_Stats, m_Colors);
  rlImGuiEnd();

  if (m_Settings.resetRequested) {
    Reset();
    m_Settings.resetRequested = false;
  }

  if (m_Settings.rerunRequested) {
    Rerun();
    m_Settings.rerunRequested = false;
  }
}

void Solver::Reset() {
  m_Grid.ClearWalls();
  m_Solver->Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
  m_Stats = VizStats{};
  m_Accumulator = 0.0f;
  m_Started = false; // enbale edit
  m_Colors = GridColors{};
  m_Settings.paused = false;

  return;
}

void Solver::Rerun() {
  m_Solver->Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
  m_Stats = VizStats{};
  m_Accumulator = 0.0f;
  m_Settings.paused = false; // so it doesn't restart into a paused state
}

void Solver::PlaceOrToggle() {

  if (ImGui::GetIO().WantCaptureMouse)
    return;

  Rectangle bounds{GetBounds()};
  Vector2 cell = m_Grid.ScreenToCell(GetMousePosition(), bounds);
  if (cell.x < 0)
    return;
  int cx{static_cast<int>(cell.x)}, cy{static_cast<int>(cell.y)};

  bool placingStart{IsKeyDown(KEY_S)};
  bool placingEnd{IsKeyDown(KEY_E)};

  if (placingStart && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    m_Grid.SetStart(cx, cy);
    return;
  }
  if (placingEnd && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    m_Grid.SetEnd(cx, cy);
    return;
  }

  if (placingStart || placingEnd)
    return;

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    m_Grid.SetWall(cx, cy, true);
  else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
    m_Grid.SetWall(cx, cy, false);
}