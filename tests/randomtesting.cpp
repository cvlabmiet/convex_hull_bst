/// @file randomtesting.cpp
/// @brief Main function for testing new algorithm versus algo from CGAl with
///        random generated points
/// @author Dmitry Matrokhin
/// @date 2018/06/12

#include <iostream>
#include <random>
#include <set>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/ch_akl_toussaint.h>
#include <CGAL/ch_graham_andrew.h>

#include "bsthull.h"
#include "point.h"

/// @brief Gets randomly generated points in square
std::vector<Point> getRandomPoints(std::mt19937& i_gen, size_t n) {
  std::uniform_real_distribution<double> dist;
  std::vector<Point> result;
  result.reserve(n);
  for (size_t i = 0; i < n; ++i) result.emplace_back(dist(i_gen), dist(i_gen));
  return result;
}

/// @brief Determines if convex hulls are equal or not
///        There can be a shift in first convex hull relative to second one
template <class TContainer1, class TContainer2, class TPred>
bool areEqualHulls(const TContainer1& first, const TContainer2& second,
                   TPred pred) {
  if (first.size() != second.size()) return false;

  auto it = std::find_if(first.begin(), first.end(), [&](const Point& point) {
    return pred(point, *second.begin());
  });
  if (it == first.end()) return false;

  for (const auto& p : second) {
    if (!pred(*it, p)) return false;
    ++it;
    if (it == first.end()) it = first.begin();
  }
  return true;
}

/// @brief Checks equality of points with epsilon = 1e-9
bool areEqualPoints(const Point& left, const Point& right) {
  const double eps = 1e-9;
  return abs(left.x() - right.x()) < eps && abs(left.y() - right.y()) < eps;
}

/// @brief Main function, does random testing of new algorithm
int main() {
  std::mt19937 gen(19);
  const size_t testsNum = 100;
  const size_t pointsNum = 1000;

  for (size_t test = 0; test < testsNum; ++test) {
    const auto points = getRandomPoints(gen, pointsNum);

    std::vector<Point> expected;
    CGAL::ch_akl_toussaint(points.begin(), points.end(),
                           std::back_inserter(expected));

    const auto actual = algorithms::BstConvexHull::Create(points);

    if (!areEqualHulls(expected, actual.GetPoints(),
                       [&](const Point& right, const Vector& left) {
                         return areEqualPoints(actual.GetCenter() + left,
                                               right);
                       })) {
      std::cerr << "bst error\n" << test;
      return 1;
    }
    std::cout << "test " << test << " passed\n";
  }

  return 0;
}
