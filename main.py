from library import read
from TabuSearch.basic import Basic as tabu
from ACO.optimized import Optimized as aco
from multiprocessing import freeze_support


if __name__ == '__main__':
    freeze_support()
    
    dic, cid = read("dataset/qa194.tsp")
    
    a = aco(ants=100, multiprocess=1, time_max=1)

    print(a.run())