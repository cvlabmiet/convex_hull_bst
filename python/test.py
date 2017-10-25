from matplotlib import pyplot as plt
import random
from subprocess import call


def generatePoints(size):
   return [(random.random(), random.random()) for i in range(size)]

def generateInput(sizes):
   with open("in.txt", 'w') as f:
      f.write(str(len(sizes)) + '\n')
      for size in sizes:
         f.write(str(size) + '\n')

         for x, y in generatePoints(size):
            f.write(str(x) + ' ' + str(y) + '\n')

def runTest():
   call([".\convex_hull\Release\convex_hull.exe"])

def readAlgoNames():
   with open("algoNames.txt", 'r') as f:
      return list(f.readlines())

def readResult():
   with open("out.txt", 'r') as f:
      return [ list(map(float, line.split())) for line in f]

def main():
   x = range(1000, 100000, 1000)
   generateInput(x)
   runTest()
   y = readResult()
   g = plt.plot(x, y)
   plt.legend(g, readAlgoNames())

   plt.show()

main()
