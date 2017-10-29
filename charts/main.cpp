#include <algorithm>
#include <chrono>
#include <fstream>
#include <vector>

#include "point.h"
#include "bsthull.h"
#include "graham.h"
#include "chan.h"


template<class TFn>
double measureAlgo(TFn fn, const std::vector<Point>& points)
{
   using namespace std::chrono;
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

   std::ofstream("algoNames.txt") << "new algo\ngraham scan\nchan\n";

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
         << measureAlgo(algorithms::GrahamScan, points) << ' '
         << measureAlgo(algorithms::Chan, points) << '\n';
   }

   return 0;
}
