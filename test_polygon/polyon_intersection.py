import os
import cv2
import numpy as np
from sympy import Point, Polygon
from sympy import symbols
from sympy.plotting import plot


if __name__ == "__main__":
    
    # creating points using Point()
    polygon_list_1 = [(0, 0), (1, 0), (1, 1), (0, 1), (0, 0)]
    polygon_list_2 = [(0, 0), (2, 0), (2, 2), (0, 2), (0, 0)]
    p1, p2, p3, p4, p5 = map(Point, polygon_list_1)
    poly1 = Polygon(p1, p2, p3, p4, p5)

    p5, p6, p7, p8, p9 = map(Point, polygon_list_2)
    
    # creating polygons using Polygon()
    poly2 = Polygon(p5, p6, p7, p8, p9)
    
    # using intersection()
    isIntersection = poly1.intersection(poly2)
    
    print(isIntersection)
    # print(list(map(list, isIntersection)))
