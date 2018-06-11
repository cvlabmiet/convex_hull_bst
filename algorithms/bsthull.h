/// @file bsthull.h
/// @brief Definition of BstConvexHull class
///        It is used to compute convex hull by using binary search tree (BST)
/// @author Dmitry Matrokhin
/// @date 2018/06/12

#ifndef __BSTHULL_H
#define __BSTHULL_H

#include <set>
#include <vector>

#include "point.h"

namespace algorithms {
namespace impl {
/// @brief Counter-clockwise predicate
bool ccw(const Vector& a, const Vector& b, const Vector& c) noexcept {
  return (b.x() * c.y() - c.x() * b.y()) - (a.x() * c.y() - c.x() * a.y()) +
             (a.x() * b.y() - b.x() * a.y()) >
         0;
}

/// @brief Vectors comparator, used to store them in BST
struct less {
  bool operator()(const Vector& left, const Vector& right) const noexcept {
    const bool leftUp = 0 < left.y();
    const bool rightUp = 0 < right.y();
    if (leftUp != rightUp) return leftUp < rightUp;
    if (left.y() == right.y() && left.y() == 0)
      return (0 < left.x()) < (0 < right.x());

    return ccw(left, right, Vector(0.0, 0.0));
  }
};

/// @brief Set wrapper, contains useful methods for cyclic iterating over set
struct CyclicSet {
  using settype = std::set<Vector, less>;
  using iterator = settype::iterator;

  auto upper_bound(const Vector& vect) const noexcept {
    const auto it = innerset.upper_bound(vect);
    return it != innerset.end() ? it : innerset.begin();
  }

  auto next(const iterator& it) const noexcept {
    const auto nextIt = std::next(it);
    return nextIt != innerset.end() ? nextIt : innerset.begin();
  }

  auto prev(const iterator& it) const noexcept {
    return it != innerset.begin() ? std::prev(it) : std::prev(innerset.end());
  }

  void erase(const iterator& it) noexcept { innerset.erase(it); }

  void emplace_hint(const iterator& it, const Vector& vect) noexcept {
    innerset.emplace_hint(it, vect);
  }

  void emplace(const Vector& vect) noexcept { innerset.emplace(vect); }

  settype innerset;
};
}  // namespace impl

/// @brief Convex hull calculator, uses new algorithm based on BST
struct BstConvexHull {
  BstConvexHull(Point center) : center(center) {}

  /// @brief Function to add new point to current convex hull
  void AddPoint(const Point& pt) {
    using impl::ccw;

    const Vector vector = pt - center;
    auto next = hull.upper_bound(vector);
    auto prev = hull.prev(next);

    if (ccw(*prev, vector, *next)) {
      while (true) {
        const auto afterNext = hull.next(next);
        if (ccw(vector, *next, *afterNext)) break;
        hull.erase(next);
        next = afterNext;
      }

      while (true) {
        const auto beforePrev = hull.prev(prev);
        if (ccw(*beforePrev, *prev, vector)) break;
        hull.erase(prev);
        prev = beforePrev;
      }
      hull.emplace_hint(prev, vector);
    }
  }

  /// @brief Computes convex hull from initial vector of points
  static BstConvexHull Create(const std::vector<Point>& points) {
    const auto minMaxIt =
        std::minmax_element(points.begin(), points.end(),
                            [](const Point& left, const Point& right) {
                              return left.x() < right.x();
                            });

    const Point minPoint = *minMaxIt.first;
    const Point maxPoint = *minMaxIt.second;

    const Point furthestPoint = [&]() {
      Point result = maxPoint;
      double furthestDist = 0;
      const auto divideVect = maxPoint - minPoint;
      for (const Point& point : points) {
        const double dist = abs((point - minPoint) * divideVect);
        if (dist > furthestDist) {
          result = point;
          furthestDist = dist;
        }
      }
      return result;
    }();

    BstConvexHull convexHull{
        Point((minPoint.x() + maxPoint.x() + furthestPoint.x()) / 3,
              (minPoint.y() + maxPoint.y() + furthestPoint.y()) / 3)};

    convexHull.hull.emplace(minPoint - convexHull.center);
    convexHull.hull.emplace(maxPoint - convexHull.center);
    convexHull.hull.emplace(furthestPoint - convexHull.center);

    for (const Point& point : points) convexHull.AddPoint(point);

    return convexHull;
  }

  /// @brief Returns set of vectors that are representing convex hull
  /// @note For getting points that are in convex hull, you need to sum this
  ///       vector with the center, see GetCenter function
  const impl::CyclicSet::settype& GetPoints() const noexcept {
    return hull.innerset;
  }

  /// @brief Gets approximate center of the convex hull
  const Point& GetCenter() const noexcept { return center; }

 private:
  impl::CyclicSet hull;
  const Point center;
};

}  // namespace algorithms

#endif // __BSTHULL_H
