from matplotlib import pyplot as plt
from matplotlib.font_manager import FontProperties
from subprocess import call
import random
import sys
import math
from scipy.spatial import ConvexHull
from shapely import geometry

font = FontProperties()
font.set_family('Times New Roman')
font.set_size(12)


def genPointsOnCircle(size):
   for i in range(size):
      angle = random.uniform(0, 2 * math.pi)
      yield (math.cos(angle), math.sin(angle))


def genPointsInCircle(size):
   for i in range(size):
      angle = random.uniform(0, 2 * math.pi)
      radius = random.random()
      yield (radius * math.cos(angle), radius * math.sin(angle))


def genPointsNormal(size):
   return [(random.gauss(0, 1), random.gauss(0, 1)) for i in range(size)]



def triangleArea(triangle):
   def distance(p1, p2):
      return math.hypot(p1[0]-p2[0], p1[1]-p2[1])

   a, b, c = triangle
   first = distance(a, b)
   second = distance(b, c)
   thrid = distance(c, a)
   p = 0.5 * (first + second + thrid)
   return math.sqrt(p * (p - first) * (p - second) * (p - thrid))


def getTriangles(points):
   p = [points[i] for i in ConvexHull(points).vertices]

   t = [(p[0], p[i], p[i+1]) for i in range(1, len(p)-1) ]
   w = [triangleArea(x) for x in t]
   return (t, w)


def getRandomInTriangle(triangle):
   A, B, C = triangle
   r1, r2 = random.random(), random.random()
   return ((1 - math.sqrt(r1)) * A[0] + (math.sqrt(r1) * (1 - r2)) * B[0] + (math.sqrt(r1) * r2) * C[0],
           (1 - math.sqrt(r1)) * A[1] + (math.sqrt(r1) * (1 - r2)) * B[1] + (math.sqrt(r1) * r2) * C[1])


def genPointsWithConvexHullPercent(size, percent):
   pointsOnHull = list(genPointsOnCircle(round(size * percent)))

   triangles, weights = getTriangles(pointsOnHull)
   pointsInHull = [getRandomInTriangle(t) for t in random.choices(triangles, weights=weights, k = size - len(pointsOnHull))]

   res = pointsOnHull + pointsInHull
   random.shuffle(res)
   return res


def genInput(sizes, genFunc):
   with open("in.txt", 'w') as f:
      f.write(str(len(sizes)) + '\n')
      for size in sizes:
         points = list(genFunc(size))
         f.write(str(len(points)) + '\n')
         for x, y in points:
            f.write(str(x) + ' ' + str(y) + '\n')


def runTest(executable):
   call([executable])


def readAlgoNames():
   with open("algoNames.txt", 'r') as f:
      return list(f.readlines())


def readResult():
   with open("out.txt", 'r') as f:
      return list(zip(*[ list(map(float, line.split())) for line in f]))

def plotAndSave(x, values, names, labelx, labely, filename):
   linestyles = ['k--', 'k-']
   g = []
   for i in range(len(values)):
      g.append(plt.plot(x, values[i], linestyles[i])[0])

   plt.yticks(fontname = "Times New Roman", fontsize = 10)
   plt.xticks(fontname = "Times New Roman", fontsize = 10)
   plt.legend(g, readAlgoNames(), prop=font)
   plt.xlabel(labelx, fontproperties=font)
   plt.ylabel(labely, fontproperties=font)
   plt.grid()

   plt.savefig(filename, bbox_inches='tight')


def new_comparison(executable):
   x = [0.1, 0.5, 1, 1.5, 2, 2.5, 3]
   print(list(x))
   l = [10000, 25000, 50000, 75000, 100000, 250000, 500000, 1000000]
   print(l)
   for n in l:
      plt.figure()
      genInput(x, lambda percent : genPointsWithConvexHullPercent(n, percent / 100))
      runTest(executable)
      y = readResult()
      print([f[1] / f[0] for f in zip(*y)])

      plotAndSave(x, y, readAlgoNames(), 'процент', 'время (сек)', 'comparison2_' + str(n) + '.svg')


def classic_comparison(executable):
   x = [1000, 2500, 5000, 7500, 10000, 25000, 50000, 75000, 100000]
   plt.figure()
   genInput(x, genPointsNormal)
   runTest(executable)

   plotAndSave(x, readResult(), readAlgoNames(), 'количество точек', 'время (сек)', 'classic_gauss.svg')


if len(sys.argv) == 2:
   new_comparison(sys.argv[1])
else:
   print("Usage: run.py path_to_executable")

