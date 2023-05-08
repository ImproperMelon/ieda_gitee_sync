#pragma once

#include <array>
#include <queue>
#include <set>
#include <utility>
#include <vector>

#include "GDSPloter.h"
#include "pgl.h"

namespace icts {
using std::array;
using std::make_pair;
using std::pair;
using std::queue;
using std::set;
using std::vector;

class GridGraph {
 public:
  GridGraph() : _row_num(0), _col_num(0) {}
  GridGraph(int row_num, int col_num)
      : _grids(row_num, vector<bool>(col_num, 0)),
        _row_num(row_num),
        _col_num(col_num) {}

  // getter
  int get_row_num() const { return _row_num; }
  int get_col_num() const { return _col_num; }

  // setter
  void set_row_num(int row_num) { _row_num = row_num; }
  void set_col_num(int col_num) { _col_num = col_num; }

  // operator
  Rectangle findPlacedLocation(Rectangle& rect) const;
  vector<Rectangle> adjacentRectangles(const Rectangle& rect,
                                       const int& interval = 1) const;

  bool placeable(const Rectangle& rect) const;
  bool withinBoundary(const Rectangle& rect) const;
  bool withinBoundary(const Point& point) const;
  bool empty(const Point& point) const;
  void setBlockage(const Rectangle& rect);
  void setBlockage(const Point& point);
  Point legalization(const Point& point) const;

 private:
  vector<Point> getPoints(const Rectangle& rect) const;

 private:
  vector<vector<bool>> _grids;
  int _row_num;
  int _col_num;
};

inline bool GridGraph::withinBoundary(const Rectangle& rect) const {
  return withinBoundary(Point(gtl::xl(rect), gtl::yl(rect))) &&
         withinBoundary(Point(gtl::xh(rect), gtl::yh(rect)));
}

inline bool GridGraph::withinBoundary(const Point& point) const {
  int x = point.x();
  int y = point.y();
  return (0 <= y && y < _row_num) && (0 <= x && x < _col_num);
}

inline bool GridGraph::empty(const Point& point) const {
  int x = point.x();
  int y = point.y();
  return _grids[y][x] == 0;
}

}  // namespace icts