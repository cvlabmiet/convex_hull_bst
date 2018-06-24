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


def generate_points_on_circle(size):
    for i in range(size):
        angle = random.uniform(0, 2 * math.pi)
        yield (math.cos(angle), math.sin(angle))


def generate_points_in_circle(size):
    for i in range(size):
        angle = random.uniform(0, 2 * math.pi)
        radius = random.random()
        yield (radius * math.cos(angle), radius * math.sin(angle))


def generate_points_with_normal_dist(size):
    return [(random.gauss(0, 1), random.gauss(0, 1)) for i in range(size)]


def triangle_area(triangle):
    def distance(p1, p2):
        return math.hypot(p1[0] - p2[0], p1[1] - p2[1])

    a, b, c = triangle
    first = distance(a, b)
    second = distance(b, c)
    third = distance(c, a)
    p = 0.5 * (first + second + third)
    return math.sqrt(p * (p - first) * (p - second) * (p - third))


def triangles_from_hull(points):
    p = [points[i] for i in ConvexHull(points).vertices]

    t = [(p[0], p[i], p[i + 1]) for i in range(1, len(p) - 1)]
    w = [triangle_area(x) for x in t]
    return t, w


def random_point_in_triangle(triangle):
    a, b, c = triangle
    r1, r2 = random.random(), random.random()
    x = (1 - math.sqrt(r1)) * a[0] + (math.sqrt(r1) * (1 - r2)) * b[0] + \
        (math.sqrt(r1) * r2) * c[0]
    y = (1 - math.sqrt(r1)) * a[1] + (math.sqrt(r1) * (1 - r2)) * b[1] + \
        (math.sqrt(r1) * r2) * c[1]
    return x, y


def generate_points_with_percent_on_hull(size, percent):
    points_on_hull = list(generate_points_on_circle(round(size * percent)))

    triangles, weights = triangles_from_hull(points_on_hull)
    in_hull_size = size - len(points_on_hull)
    points_in_hull = [random_point_in_triangle(t) for t in random.choices(
        triangles, weights=weights, k=in_hull_size)]

    res = points_on_hull + points_in_hull
    random.shuffle(res)
    return res


def generate_input(sizes, genFunc):
    with open("in.txt", 'w') as f:
        f.write(str(len(sizes)) + '\n')
        for size in sizes:
            points = list(genFunc(size))
            f.write(str(len(points)) + '\n')
            for x, y in points:
                f.write(str(x) + ' ' + str(y) + '\n')


def read_algo_names():
    with open("algoNames.txt", 'r') as f:
        return list(f.readlines())


def read_result():
    with open("out.txt", 'r') as f:
        return list(zip(*[list(map(float, line.split())) for line in f]))


def plot_and_save(x, values, names, labelx, labely, filename):
    linestyles = ['k--', 'k-']
    g = []
    for i in range(len(values)):
        g.append(plt.plot(x, values[i], linestyles[i])[0])

    plt.yticks(fontname="Times New Roman", fontsize=10)
    plt.xticks(fontname="Times New Roman", fontsize=10)
    plt.legend(g, names, prop=font)
    plt.xlabel(labelx, fontproperties=font)
    plt.ylabel(labely, fontproperties=font)
    plt.grid()

    plt.savefig(filename, bbox_inches='tight')


def new_comparison(executable):
    percentage_on_hull = [0.1, 0.5, 1, 1.5, 2, 2.5, 3]
    print(percentage_on_hull)
    points_num = [10000, 25000, 50000, 75000, 100000, 250000, 500000, 1000000]
    print(points_num)
    for n in points_num:
        generate_input(percentage_on_hull,
                       lambda percent:
                       generate_points_with_percent_on_hull(n, percent / 100))
        call([executable])
        y = read_result()

        print([f[1] / f[0] for f in zip(*y)])
        plt.figure()
        plot_and_save(percentage_on_hull, y, read_algo_names(),
                      'процент', 'время (сек)',
                      'comparison_' + str(n) + '.svg')


def classic_comparison(executable):
    x = [1000, 2500, 5000, 7500, 10000, 25000, 50000, 75000, 100000]
    plt.figure()
    generate_input(x, generate_points_with_normal_dist)
    call([executable])

    plot_and_save(x, read_result(), read_algo_names(),
                  'количество точек', 'время (сек)', 'comparison_gauss.svg')


if len(sys.argv) == 2:
    new_comparison(sys.argv[1])
else:
    print("Usage: run.py path_to_executable")
