from matplotlib import pyplot as plt
from subprocess import call
import random
import sys
import math

def genPointsOnCircle(size):
   for i in range(size):
      angle = random.uniform(0, 2 * math.pi)
      yield (math.cos(angle), math.sin(angle))

def genPointsInCircle(size):
   for i in range(size):
      angle = random.uniform(0, 2 * math.pi)
      radius = random.random()
      yield (radius * math.cos(angle), radius * math.sin(angle))

def genPointsByPercentInCircle(size, percent):
   pointsOnCircle = round(size * percent)
   return list(genPointsOnCircle(pointsOnCircle)) + list(genPointsInCircle(size - pointsOnCircle))


def genPointsSquare(size):
   return [(random.random(), random.random()) for i in range(size)]


def genInput(sizes, genFunc):
   with open("in.txt", 'w') as f:
      f.write(str(len(sizes)) + '\n')
      for size in sizes:
         points = genFunc(size)
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
      return [ list(map(float, line.split())) for line in f]


def main(executable):
   x = range(10, 100, 10)
   genInput(x, lambda percent : genPointsByPercentInCircle(100000, percent / 100))
   runTest(executable)
   y = readResult()
   g = plt.plot(x, y)
   plt.legend(g, readAlgoNames())

   plt.grid()
   plt.show()

if len(sys.argv) == 2:
   main(sys.argv[1])
else:
   print("Usage: run.py path_to_executable")
