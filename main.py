from library import read
from TabuSearch.basic import Basic as tabu
from ACO.optimized import Optimized as aco

dic, cid = read("dataset/qa194.tsp")

a = aco(ants=100, multiprocess=1, time_max=60)

print(a.run())