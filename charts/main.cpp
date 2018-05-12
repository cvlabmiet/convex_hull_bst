#include <algorithm>
#include <chrono>
#include <fstream>
#include <vector>

#include <CGAL/ch_graham_andrew.h>

#include "point.h"
#include "bsthull.h"

template<class TFn, class ...TArgs>
double measureAlgo(TFn fn, size_t testsNum, TArgs&&... args)
{
   using namespace std::chrono;

   const auto tStart = high_resolution_clock::now();

   for (size_t test = 0; test < testsNum; ++test)
      (void) fn(std::forward<TArgs>(args)...);

   const auto tEnd = high_resolution_clock::now();

   return duration_cast<duration<double>>(tEnd - tStart).count() / testsNum;
}

int main()
{
   std::ifstream fin("in.txt");
   std::ofstream fout("out.txt");

   std::ofstream("algoNames.txt") << "Graham scan\nNew algorithm";

   size_t testsNum;
   fin >> testsNum;

   for (size_t test = 0; test < testsNum; ++test)
   {
      int pointsNum;
      fin >> pointsNum;

      std::vector<Point> points;
      points.reserve(pointsNum);
      for (int i = 0; i < pointsNum; ++i)
      {
         double x, y;
         fin >> x >> y;
         points.emplace_back(x, y);
      }

      const size_t runsNum = std::max(20, 1'000'000 / pointsNum);
      std::vector<Point> result(pointsNum);

      // heat up cache for algo
      measureAlgo(CGAL::ch_graham_andrew<std::vector<Point>::iterator, std::vector<Point>::iterator>,
         runsNum, points.begin(), points.end(), result.begin());
      fout << measureAlgo(CGAL::ch_graham_andrew<std::vector<Point>::iterator, std::vector<Point>::iterator>,
         runsNum, points.begin(), points.end(), result.begin()) << ' ';

      // heat up cache for algo
      measureAlgo(algorithms::BstConvexHull::Create, runsNum, points);
      fout << measureAlgo(algorithms::BstConvexHull::Create, runsNum, points) << '\n';
   }

   return 0;
}
