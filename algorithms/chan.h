#include <vector>
#include <algorithm>

#include "graham.h"
#include "point.h"

namespace algorithms
{
double getMult(const Point& point0, const Point& point1, const Point& point2)
{
   return (point1 - point0) * (point2 - point1);
}
bool isLeft(const Point& point0, const Point& point1, const Point& point2)
{
   return getMult(point0, point1, point2) > 0.0;
}
bool isRight(const Point& point0, const Point& point1, const Point& point2)
{
   return getMult(point0, point1, point2) < 0.0;
}

size_t rightTangent(Point point, const std::vector<Point>& hull)
{
   if (hull.size() == 1 || (isRight(point, hull[1], hull[0]) && !isLeft(point, hull.back(), hull[0])))
      return 0;

   size_t left = 0;
   size_t right = hull.size();
   while (true)
   {
      const size_t middle = (left + right) / 2;
      // TODO: refactor
      const bool prevToMid = isRight(point, hull[(middle + 1)%hull.size()], hull[middle]);
      if (prevToMid && !isLeft(point, hull[(middle - 1 + hull.size()) % hull.size()], hull[middle]))
         return middle;          // V[c] is the maximum tangent point

      const bool leftToNext = isLeft(point, hull[left + 1], hull[left]);
      if (leftToNext)
      {
         if (prevToMid)
            right = middle;
         else if (isLeft(point, hull[left], hull[middle]))
            right = middle;
         else
            left = middle;
      }
      else
      {
         if (!prevToMid)
            left = middle;
         else if (isRight(point, hull[left], hull[middle]))
            right = middle;
         else
            left = middle;
      }
   }
}

std::pair<size_t, size_t> nextPointInHulls(
   const std::vector<std::vector<Point>>& hulls,
   const size_t curHullIdx,
   const size_t curPointIdx)
{
   const Point curPoint = hulls[curHullIdx][curPointIdx];
   auto next = std::make_pair(curHullIdx, (curPointIdx + 1) % hulls[curHullIdx].size());

   for (size_t hullIdx = 0; hullIdx < hulls.size(); ++hullIdx)
   {
      if (hullIdx == curHullIdx)
         continue;
      const size_t bestIdx = rightTangent(curPoint, hulls[hullIdx]);
      const Point curNextPoint = hulls[next.first][next.second];
      const Point newNextPoint = hulls[hullIdx][bestIdx];
      const double mult = getMult(curPoint, curNextPoint, newNextPoint);
      if (mult < 0.0 || (mult == 0.0 &&
         (curNextPoint - curPoint).length() < (newNextPoint - curPoint).length()))
         next = std::make_pair(hullIdx, bestIdx);
   }
   return next;
}

std::vector<Point> Chan(std::vector<Point> points)
{
   for (size_t t = 0; t < points.size(); ++t)
   {
      const size_t one = 1;
      size_t m = points.size();
      const size_t pow2 = (one << t);
      if (pow2 < points.size() && (points.size() >> pow2) > 0)
         m = std::min((one << pow2), points.size());

      std::vector<std::vector<Point>> hulls;
      hulls.reserve(points.size() / m + 1);

      for (size_t chunkStart = 0; chunkStart < points.size(); chunkStart += m)
      {
         auto chunkBegin = points.begin() + chunkStart;
         hulls.emplace_back(GrahamScanInPlace(chunkBegin,
               ((chunkStart + m < points.size()) ?
                  (chunkBegin + m) :
                  points.end())));
      }
      // TODO: find not just leftmost, but leftmost, that is at the bottom
      size_t curHullIdx = 0;
      size_t curPointIdx = 0;
      for (size_t hullNum = 1; hullNum < hulls.size(); ++hullNum)
         if (hulls[hullNum].front().x < hulls[curHullIdx].front().x)
            curHullIdx = hullNum;

      std::vector<Point> res;
      res.reserve(m);
      res.push_back(hulls[curHullIdx][curPointIdx]);

      const size_t firstHullIdx = curHullIdx;

      for (size_t i = 0; i <= m; ++i)
      {
         auto p = nextPointInHulls(hulls, curHullIdx, curPointIdx);
         if (firstHullIdx == p.first && p.second == 0)
            return res;
         curHullIdx = p.first;
         curPointIdx = p.second;

         res.push_back(hulls[curHullIdx][curPointIdx]);
      }
   }
   return {};
}
}
