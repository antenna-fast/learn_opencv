from shapely.geometry import Point as Point, Polygon

coords = ((0., 0.), (0., 1.), (1., 1.), (1., 0.), (0., 0.))
polygon = Polygon(coords)

print(polygon.area)
