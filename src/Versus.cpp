#include <Versus.hpp>
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

Versus::Versus(StateMachine &machine)
    : State(machine), m_Grid(GetScreenWidth() / 2, GetScreenHeight()) {

  m_Grid.SetStart(0, 0);
  m_Grid.SetEnd(m_Grid.Cols() - 1, m_Grid.Rows() - 1);
}

void Versus::HandleInput() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    RequestChange(StateID::MainMenu);
    return;
  }

  if (!m_Started && m_Settings.paused) {
    PlaceOrToggle();
    if (IsKeyPressed(KEY_SPACE)) {
      m_Astar.Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
      m_Dijkstra.Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
      m_Started = true;
    }
  }
}

Rectangle Versus::LeftBounds() const {
  float w{static_cast<float>(GetScreenWidth())};
  float h{static_cast<float>(GetScreenHeight())};
  return {0, 0, w / 2.0f, h};
}

Rectangle Versus::RightBounds() const {
  float w{static_cast<float>(GetScreenWidth())};
  float h{static_cast<float>(GetScreenHeight())};
  return {w / 2.0f + 2.0f, 0, w / 2.0f - 2.0f, h};
}

void Versus::PlaceOrToggle() {

  if (ImGui::GetIO().WantCaptureMouse)
    return;

  Vector2 mouse{GetMousePosition()};
  Rectangle bounds{(mouse.x < GetScreenWidth() / 2.0f) ? LeftBounds()
                                                       : RightBounds()};
  Vector2 cell{m_Grid.ScreenToCell(mouse, bounds)};

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

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    m_PaintValue = !m_Grid.IsWall(cx, cy);
    m_Grid.SetWall(cx, cy, m_PaintValue);
  } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    m_Grid.SetWall(cx, cy, m_PaintValue);
  } else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
    m_Grid.SetWall(cx, cy, false);
  }
}

void Versus::Reset() {
  m_Grid.ClearWalls();
  m_Astar.Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
  m_Dijkstra.Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
  m_StatsA = VizStats{};
  m_StatsB = VizStats{};
  m_Accumulator = 0.0f;
  m_Started = false;
  m_Settings.paused = false;
}

void Versus::Rerun() {
  m_Astar.Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
  m_Dijkstra.Init(m_Grid, m_Grid.GetStart(), m_Grid.GetEnd());
  m_StatsA = VizStats{};
  m_StatsB = VizStats{};
  m_Accumulator = 0.0f;
  m_Settings.paused = false;
}
void Versus::Update(float dt) {

  if (IsKeyPressed(KEY_BACKSPACE)) {
    RequestChange(StateID::MainMenu);
    return;
  }
  if (!m_Started)
    return;
  if (m_Settings.paused)
    return;
  float stepInterval{1.0f / m_Settings.steps};

  m_Accumulator += dt;
  while (m_Accumulator >= stepInterval &&
         !(m_StatsA.finished && m_StatsB.finished)) {
    if (!m_StatsA.finished)
      m_StatsA.finished = m_Astar.Step();
    if (!m_StatsB.finished)
      m_StatsB.finished = m_Dijkstra.Step();
    m_Accumulator -= stepInterval;
  }

  if (!m_StatsA.finished)
    m_StatsA.elapsedTime += dt;
  if (!m_StatsB.finished)
    m_StatsB.elapsedTime += dt;

  m_StatsA.nodesVisited = static_cast<int>(m_Astar.GetVisited().size());
  m_StatsB.nodesVisited = static_cast<int>(m_Dijkstra.GetVisited().size());

  if (m_StatsA.finished) {
    m_StatsA.found = m_Astar.Found();
    m_StatsA.pathLength = static_cast<int>(m_Astar.GetVisited().size());
  }
  if (m_StatsB.finished) {
    m_StatsB.found = m_Dijkstra.Found();
    m_StatsB.pathLength = static_cast<int>(m_Dijkstra.GetVisited().size());
  }
}

void Versus::Draw() {
  Rectangle left{LeftBounds()};
  Rectangle right{RightBounds()};

  const auto *visitedA{m_Started ? &m_Astar.GetVisited() : nullptr};
  const auto *pathA{(m_Started && m_StatsA.finished && m_StatsA.found)
                        ? &m_Astar.GetPath()
                        : nullptr};
  m_Grid.Draw(left, m_ColorsA, visitedA, pathA);

  const auto *visitedB{m_Started ? &m_Dijkstra.GetVisited() : nullptr};
  const auto *pathB{(m_Started && m_StatsB.finished && m_StatsB.found)
                        ? &m_Dijkstra.GetPath()
                        : nullptr};
  m_Grid.Draw(right, m_ColorsB, visitedB, pathB);

  DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(),
           GRAY);

  rlImGuiBegin();
  DrawVizPanel(m_Settings, m_StatsA, m_ColorsA, m_StatsB, m_ColorsB);
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