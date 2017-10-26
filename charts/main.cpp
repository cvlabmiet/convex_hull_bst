#include <cstdio>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <set>
#include <functional>
#include <fstream>
#include <numeric>
#include <queue>
#include <random>
#include <exception>

#include "point.h"
#include "bsthull.h"
#include "graham.h"

using namespace std::chrono;

template<class TFn>
double measureAlgo(TFn fn, const std::vector<Point>& points)
{
   const size_t minTestsNum = 10;
   const size_t testsNum = std::max(minTestsNum, 100'000U / points.size());
   const auto tStart = high_resolution_clock::now();

   for (size_t test = 0; test < testsNum; ++test)
      const auto res = fn(points);

   const auto tEnd = high_resolution_clock::now();

   return duration_cast<duration<double>>(tEnd - tStart).count() / testsNum;
}

int main()
{
   std::ifstream fin("in.txt");
   std::ofstream fout("out.txt");

   std::ofstream("algoNames.txt") << "new algo\ngraham scan\n";

   size_t testsNum;
   fin >> testsNum;
   for (size_t test = 0; test < testsNum; ++test)
   {
      int pointsNum;
      fin >> pointsNum;
      std::vector<Point> points(pointsNum);
      for (Point& point : points) fin >> point.x >> point.y;

      fout
         << measureAlgo(algorithms::BstConvexHull::Create, points) << ' '
         << measureAlgo(algorithms::GrahamScan, points) << '\n';
   }

   return 0;
}
