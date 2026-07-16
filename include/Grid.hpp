#pragma once

#include <raylib.h>
#include <vector>

enum class CellType : uint8_t {
  Empty,
  Wall,
};

struct GridColors {
  Color empty = RAYWHITE;
  Color wall = DARKGRAY;
  Color start = GREEN;
  Color end = RED;
  Color visited = SKYBLUE;
  Color path = GOLD;
  Color gridLine = LIGHTGRAY;
  Color visitedStart = SKYBLUE; // color of earliest-visited cells
  Color visitedEnd = DARKBLUE;  // color of most-recently-visited cells
};

class Grid {
public:
  Grid() = default;
  Grid(int cols, int rows);

  float CellSize(Rectangle &bound) const;
  Vector2 ScreenToCell(Vector2 mouse, Rectangle &bounds) const;
  void Draw(Rectangle bounds, const GridColors &colors,
            const std::vector<Vector2> *visited = nullptr,
            const std::vector<Vector2> *path = nullptr) const;

  void SetWall(int x, int y, bool isWall);
  bool IsWall(int x, int y) const;
  void ClearWalls();

  void SetStart(int x, int y) {
    m_Start = {static_cast<float>(x), static_cast<float>(y)};
  }
  void SetEnd(int x, int y) {
    m_End = {static_cast<float>(x), static_cast<float>(y)};
  }

  Vector2 GetStart() const { return m_Start; }
  Vector2 GetEnd() const { return m_End; }
  int Cols() const { return m_Cols; }
  int Rows() const { return m_Rows; }

private:
  void ComputeGridDimension(int width, int height);

private:
  std::vector<CellType> m_Cells{};
  Vector2 m_Start{};
  Vector2 m_End{};
  int m_Cols{}, m_Rows{};
  int m_Size{20};
};