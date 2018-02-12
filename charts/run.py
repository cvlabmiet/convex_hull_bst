from matplotlib import pyplot as plt
from matplotlib.font_manager import FontProperties
from subprocess import call
import random
import sys
import math

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

def genPointsByPercentInCircle(size, percent):
   pointsOnCircle = round(size * percent)
   res = list(genPointsOnCircle(pointsOnCircle)) + list(genPointsInCircle(size - pointsOnCircle))
   random.shuffle(res)
   return res


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
      return list(zip(*[ list(map(float, line.split())) for line in f]))


def main(executable):
   x = range(1, 21, 1)#[1, 2.5, 5, 7.5, 10, 12.5, 15, 17.5, 20]
   print(list(x))
   l = [1000, 2500, 5000, 7500, 10000, 25000, 50000, 75000, 100000]
   print(l)
   for n in l:
      plt.figure()
      genInput(x, lambda percent : genPointsByPercentInCircle(n, percent / 100))
      runTest(executable)
      y = readResult()
      print([f[0] / f[1] for f in y])

      linestyles = ['k--', 'k-']
      g = []
      for i in range(len(y)):
         g.append(plt.plot(x, y[i], linestyles[i])[0])

      #plt.title(str(n))
      plt.yticks(fontname = "Times New Roman", fontsize = 10)  
      plt.xticks(fontname = "Times New Roman", fontsize = 10)  
      plt.legend(g, readAlgoNames(), prop=font)
      plt.xlabel('percentage', fontproperties=font)
      plt.ylabel('time (sec)', fontproperties=font)
      plt.grid()

   plt.show()

if len(sys.argv) == 2:
   main(sys.argv[1])
else:
   print("Usage: run.py path_to_executable")

