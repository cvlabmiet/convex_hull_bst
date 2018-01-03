#include <algorithm>
#include <chrono>
#include <fstream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/ch_akl_toussaint.h>
#include <CGAL/ch_graham_andrew.h>

#include "point.h"
#include "bsthull.h"
#include "graham.h"
#include "chan.h"

using Point_2 = CGAL::Exact_predicates_inexact_constructions_kernel::Point_2;

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

   std::ofstream("algoNames.txt") << "New algorithm\nGraham scan\nakl_toussaint";

   size_t testsNum;
   fin >> testsNum;

   for (size_t test = 0; test < testsNum; ++test)
   {
      int pointsNum;
      fin >> pointsNum;

      // TODO: Remove our impl of point and try to use traits as in CGAL
      std::vector<Point> points;
      points.reserve(pointsNum);
      std::vector<Point_2> points2;
      points2.reserve(pointsNum);
      for (int i = 0; i < pointsNum; ++i)
      {
         double x, y;
         fin >> x >> y;
         points.push_back({ x, y });
         points2.emplace_back(x, y);
      }

      const size_t runsNum = std::max(10, 100'000 / pointsNum);
      std::vector<Point_2> result(pointsNum);
      if (test == 0)
      {
         for (int i = 0; i < 10; ++i)
            algorithms::BstConvexHull::Create(points);
      }
      fout
         << measureAlgo(algorithms::BstConvexHull::Create, runsNum, points) << ' '
         << measureAlgo(CGAL::ch_graham_andrew<std::vector<Point_2>::iterator, std::vector<Point_2>::iterator>,
            runsNum, points2.begin(), points2.end(), result.begin()) /*<< ' '
         << measureAlgo(CGAL::ch_akl_toussaint<std::vector<Point_2>::iterator, std::vector<Point_2>::iterator>,
            runsNum, points2.begin(), points2.end(), result.begin()) */<< '\n';
   }

   return 0;
}
