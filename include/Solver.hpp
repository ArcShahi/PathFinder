#pragma once

#include <Grid.hpp>
#include <IPathfinder.hpp>
#include <State.hpp>
#include <UI.hpp>
#include <memory>
#include <string>

enum class Algo { Astar, Dijkstra };

class Solver : public State {
public:
  Solver(StateMachine &machine, Algo algo);

  void HandleInput() override;
  void Update(float dt) override;
  void Draw() override;

private:
  void Reset();
  void Restart();
  void PlaceOrToggle(); // for shared mouse-click

private:
  Algo m_Algo{};
  Grid m_Grid{};
  std::unique_ptr<IPathFinder> m_Solver{};
  GridColors m_Colors{};
  VizSettings m_Settings{};
  VizStats m_Stats{};

  float m_Accumulator{0.0f};
  bool m_Started{false};
  bool m_PaintValue{false};
};
