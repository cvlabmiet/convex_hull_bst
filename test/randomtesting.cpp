#include <vector>
#include <random>
#include <set>
#include <iostream>

#include "bsthull.h"
#include "graham.h"
#include "chan.h"

std::vector<Point> getRandomPoints(std::mt19937& i_gen, size_t n)
{
   std::uniform_real_distribution<double> dist;
   std::vector<Point> result;
   result.reserve(n);
   for (size_t i = 0; i < n; ++i) result.push_back({ dist(i_gen), dist(i_gen) });
   return result;
}

template<class TContainer1, class TContainer2, class TPred>
bool areEqualHulls(const TContainer1& first, const TContainer2& second, TPred pred)
{
   if (first.size() != second.size()) return false;

   auto it = std::find_if(first.begin(), first.end(),
      [&](const Point& point)
      {
         return pred(point, *second.begin());
      });
   if (it == first.end())
      return false;

   for (const auto& p : second)
   {
      if (!pred(*it, p))
         return false;
      ++it;
      if (it == first.end()) it = first.begin();
   }
   return true;
}

int main()
{
   std::mt19937 gen(19);
   const size_t testsNum = 100;
   const size_t pointsNum = 1000;

   for (size_t test = 0; test < testsNum; ++test)
   {
      auto points = getRandomPoints(gen, pointsNum);

      auto resBst = algorithms::BstConvexHull::Create(points);
      auto resGraham = algorithms::GrahamScan(points);
      auto resChan = algorithms::Chan(points);

      if (!areEqualHulls(resBst.GetPoints(), resGraham,
         [&](const Point& left, const Point& right)
         {
            return left + resBst.GetCenter() == right;
         }))
      {
         std::cerr << "bst error\n" << test;
         return 1;
      }

      if (!areEqualHulls(resGraham, resChan,
         [](const Point& left, const Point& right)
         {
            return left == right;
         }))
      {
         std::cerr << "chan error\n" << test;
         return 2;
      }
   }

   return 0;
}