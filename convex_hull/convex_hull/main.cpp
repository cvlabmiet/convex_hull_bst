#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <cstdio>
#include <queue>
#include <random>
#include <chrono>

using namespace std;

struct Point
{
   double x, y;

   bool operator==(const Point& p) const { return abs(x - p.x) < 1e-9 && abs(y - p.y) < 1e-9; }
   double operator%(const Point& p) const { return x*p.x + y*p.y; }
   double operator*(const Point& p) const { return x*p.y - y*p.x; }
   Point operator/(const double value) const { return { x / value, y / value }; }
   Point operator-(const Point& p) const { return { x - p.x, y - p.y }; }
   Point operator+(const Point& p) const { return { x + p.x, y + p.y }; }
};

struct ConvexHull
{
   struct less
   {
      bool operator()(const Point& left, const Point& right) const
      {
         const bool leftUp =  0 < left.y;
         const bool rightUp = 0 < right.y;
         if (leftUp != rightUp)
            return leftUp < rightUp;
         if (left.y == right.y && left.y == 0)
            return (0 < left.x) < (0 < right.x);

         return left * right > 0.0;
      }
   };

   void AddPoint(const Point& pt)
   {
      const Point point = pt - center;
      if (convexHull.size() < 2)
      {
         convexHull.emplace(point);
         return;
      }
      auto next = getIt(convexHull.upper_bound(point));
      auto prev = prevIt(next);

      if ((point - *prev) * (*next - *prev) > 0.0)
      {
         // works not well on it and pr on different sides of tree
         convexHull.emplace_hint(prev, point);

         while ((*nextIt(next) - point) * (*next - point) > 0.0)
            next = getIt(convexHull.erase(next));
         while ((*prevIt(prev) - point) * (*prev - point) < 0.0)
            prev = prevIt(convexHull.erase(prev));
      }
   }

   ConvexHull(Point center) : center(center) {}

private:
   std::set<Point, less>::iterator getIt(std::set<Point, less>::iterator it) const noexcept
   {
      return it != convexHull.end() ? it : convexHull.begin();
   }
   std::set<Point, less>::iterator nextIt(std::set<Point, less>::iterator it) const noexcept
   {
      return getIt(std::next(it));
   }
   std::set<Point, less>::iterator prevIt(std::set<Point, less>::iterator it) const noexcept
   {
      return it != convexHull.begin() ? std::prev(it) : std::prev(convexHull.end());
   }

public: // temporary public
   Point center;
   std::set<Point, less> convexHull;
};

ConvexHull newAlgo(const std::vector<Point>& points)
{
   Point maxPoint = points[0];
   Point minPoint = points[1];
   for (const Point& point : points)
   {
      if (point.x > maxPoint.x) maxPoint = point;
      if (point.x < minPoint.x) minPoint = point;
   }
   ConvexHull convexHull{ (minPoint + maxPoint) / 2 };
   convexHull.AddPoint(minPoint);
   convexHull.AddPoint(maxPoint);

   for (const Point& point : points)
      convexHull.AddPoint(point);

   return convexHull;
}

std::vector<Point> grahamScan(std::vector<Point> points)
{
   std::vector<Point> result;
   result.reserve(points.size());
   for (size_t i = 1; i < points.size(); ++i)
      if (points[0].x > points[i].x) std::swap(points[0], points[i]);

   sort(points.begin() + 1, points.end(), [&](const Point& left, const Point& right)
   {
      return (left - points[0]) * (right - points[0]) > 0.0;
   });

   for (const auto& point : points)
   {
      while (result.size() > 2 && (result.back() - point) * (result[result.size() - 2] - result.back()) > 0.0)
         result.pop_back();
      result.push_back(point);
   }
   return result;
}

std::vector<Point> getRandomPoints(std::mt19937& i_gen, size_t n)
{
   std::uniform_real_distribution<double> dist;
   std::vector<Point> result;
   result.reserve(n);
   for (size_t i = 0; i < n; ++i) result.push_back({ dist(i_gen), dist(i_gen) });
   return result;
}

bool areEqualHulls(const ConvexHull& first, const std::vector<Point>& second)
{
   if (first.convexHull.size() != second.size()) return false;

   for (size_t beg = 0; beg < second.size(); ++beg)
   {
      auto it = first.convexHull.begin();
      bool areEqual = true;
      for (size_t i = 0; i < second.size(); ++i, ++it)
         areEqual &= ((*it + first.center) == second[(i + beg) % second.size()]);
      if (areEqual)
         return true;
   }
   return false;
}

using namespace std::chrono;

int main()
{
   std::mt19937 gen(19);
   double myTime = 0.0;
   double grahamTime = 0.0;
   const size_t testsNum = 10;
   const size_t pointsNum = 1'000'000;

   for (size_t test = 0; test < testsNum; ++test)
   {
      auto points = getRandomPoints(gen, pointsNum);

      const auto t1 = high_resolution_clock::now();
      auto res1 = newAlgo(points);

      const auto t2 = high_resolution_clock::now();
      auto res2 = grahamScan(points);
      const auto t3 = high_resolution_clock::now();

      myTime += duration_cast<duration<double>>(t2 - t1).count();
      grahamTime += duration_cast<duration<double>>(t3 - t2).count();
      /*
      if (!areEqualHulls(res1, res2))
      {
         cout << "test: " << test << '\n';
         cout << "------------------Points---------------\n";
         for (const auto& p : points)
            cout << p.x << ' ' << p.y << '\n';
         cout << "--------------------My-----------------\n";
         for (auto p : res1.convexHull)
         {
            p = p + res1.center;
            cout << p.x << ' ' << p.y << '\n';
         }
         cout << "------------------Graham---------------\n";
         for (const auto& p : res2)
            cout << p.x << ' ' << p.y << '\n';
         break;
      }
      */
   }

   cout << "new algo: " << myTime << "\ngraham: " << grahamTime << '\n';

   return 0;
}