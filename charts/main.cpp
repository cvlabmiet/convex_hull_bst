/// @file main.cpp
/// @brief Main function to run algorithms and measure their working time
/// @author Dmitry Matrokhin
/// @date 2018/06/12

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iterator>
#include <vector>

#include <CGAL/ch_graham_andrew.h>

#include "bsthull.h"
#include "point.h"

/// @brief Measures working time of the given function
template <class TFn>
double measureAlgo(TFn fn, size_t runsExternal, size_t runsInternal) {
  using namespace std::chrono;

  double bestResult = std::numeric_limits<double>::max();

  for (size_t run = 0; run < runsExternal; ++run) {
    const auto tStart = high_resolution_clock::now();
    for (size_t test = 0; test < runsInternal; ++test) fn();
    const auto tEnd = high_resolution_clock::now();
    bestResult = std::min(
        bestResult,
        duration_cast<duration<double>>(tEnd - tStart).count() / runsInternal);
  }

  return bestResult;
}

/// @brief Main function, does running of new algorithm and graham scan and
///        measures their working time
int main() {
  std::ifstream fin("in.txt");
  std::ofstream fout("out.txt");
  std::ofstream("algoNames.txt") << "Graham scan\nNew algorithm";

  size_t testsNum;
  fin >> testsNum;

  for (size_t test = 0; test < testsNum; ++test) {
    int pointsNum;
    fin >> pointsNum;

    std::vector<Point> points;
    points.reserve(pointsNum);
    for (int i = 0; i < pointsNum; ++i) {
      double x, y;
      fin >> x >> y;
      points.emplace_back(x, y);
    }

    const size_t runsExternal = 20;
    const size_t runsInternal = std::max(30, 1'000'000 / pointsNum);

    fout << measureAlgo(
                [&points]() {
                  std::vector<Point> result;
                  result.reserve(points.size());
                  CGAL::ch_graham_andrew(points.begin(), points.end(),
                                         std::back_inserter(result));
                },
                runsExternal, runsInternal)
         << ' ';

    fout << measureAlgo(
                [&points]() { algorithms::BstConvexHull::Create(points); },
                runsExternal, runsInternal)
         << '\n';
  }

  return 0;
}
