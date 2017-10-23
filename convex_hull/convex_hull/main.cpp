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

   bool operator==(const Point& p) const { return x == p.x && y == p.y; }
   double operator%(const Point& p) const { return x*p.x + y*p.y; }
   double operator*(const Point& p) const { return x*p.y - y*p.x; }
   Point operator/(const double value) const { return { x / value, y / value }; }
   Point operator-(const Point& p) const { return { x - p.x, y - p.y }; }
   Point operator+(const Point& p) const { return { x + p.x, y + p.y }; }
};

double angle(const Point& p1, const Point& p2)
{
   auto t = p2 - p1;
   return atan2(t.y, t.x);
}

std::map<double, Point> newAlgo(std::vector<Point> points)
{
   std::map<double, Point> result;
   const auto prevIt = [&](auto it) { return it == result.begin() ? prev(result.end()) : prev(it); };
   const auto nextIt = [&](auto it) { ++it; return it == result.end() ? result.begin() : it; };
   const auto getIt = [&](auto it) { return it == result.end() ? result.begin() : it; };

   // first 2 points: maxX, minX
   for (auto& point : points)
   {
      if (point.x > points[0].x) std::swap(point, points[0]);
      if (point.x < points[1].x) std::swap(point, points[1]);
   }
   auto divideVect = points[0] - points[1];
   for (size_t i = 3; i < points.size(); ++i)
   {
      if (abs((points[i] - points[1]) * divideVect) >
         abs((points[2] - points[1]) * divideVect))
         std::swap(points[i], points[2]);
   }

   Point center{ 0, 0 };
   for (size_t i = 0; i < 3U; ++i)
      center = center + points[i];
   center = center / 3;

   for (const auto& point : points)
   {
      const auto ang = angle(center, point);
      if (result.size() < 3)
      {
         result[ang] = point;
         continue;
      }

      auto it = getIt(result.upper_bound(ang));
      auto pr = prevIt(it);

      if ((point - pr->second) * (it->second - pr->second) > 0.0)
      {
         // works not well on it and pr on different sides of tree
         result.emplace_hint(it, ang, point);

         while ((nextIt(it)->second - point) * (it->second - point) > 0.0)
            it = getIt(result.erase(it));
         while ((prevIt(pr)->second - point) * (pr->second - point) < 0.0)
            pr = prevIt(result.erase(pr));
      }
   }
   return result;
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

bool areEqualHulls(const std::map<double, Point>& first, const std::vector<Point>& second)
{
   if (first.size() != second.size()) return false;

   for (size_t beg = 0; beg < second.size(); ++beg)
   {
      auto it = first.begin();
      bool areEqual = true;
      for (size_t i = 0; i < second.size(); ++i, ++it)
         areEqual &= (it->second == second[(i + beg) % second.size()]);
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
   const size_t testsNum = 100;
   const size_t pointsNum = 1000;

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

      if (!areEqualHulls(res1, res2))
      {
         cout << "test: " << test << '\n';
         cout << "------------------Points---------------\n";
         for (const auto& p : points)
            cout << p.x << ' ' << p.y << '\n';
         cout << "--------------------My-----------------\n";
         for (const auto& p : res1)
            cout << p.second.x << ' ' << p.second.y << '\n';
         cout << "------------------Graham---------------\n";
         for (const auto& p : res2)
            cout << p.x << ' ' << p.y << '\n';
         break;
      }
   }

   cout << "new algo: " << myTime << "\ngraham: " << grahamTime << '\n';

   return 0;
}