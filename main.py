from library import read
from TabuSearch.basic import Basic as tabu
from ACO.optimized import Optimized as aco
from GRASP.basic import Basic as grasp
from multiprocessing import freeze_support


if __name__ == '__main__':
    freeze_support()
    
    dic, cid = read("dataset/wi29.tsp")
    
    
    
    a = tabu(path_init= None, time_max=180, verbose=1, alpha=2)
    best = a.run()
    
    print(best)
    
    
    
    