from library import fitness, create_path, dic, cid, euclidean_distance
import random
import copy
import time

class Basic:
    
    it = 0
    it_max = 0
    time_max = 180
    alpha = 0
    best_solution = None
    verbose = False
    complex_verbose = False
    n = 0
    
    def __init__(self, path_init = None, it_max = 0, time_max = 180, verbose = False, complex_verbose = False, alpha:float = 0) -> None:
        if path_init is None:
            path_init = create_path()
        
        self.it_max = it_max
        self.time_max = time_max
        self.verbose = verbose
        self.complex_verbose = complex_verbose
        self.n = len(path_init)
        self.alpha = min(max(0, alpha), 1)
        self.best_solution = (fitness(path_init, self.n), path_init)
        
    def gen_g(self, solution, candidates):
        """
        """
        
        g_list = []
        
        for caditade in candidates:
            g_list.append((euclidean_distance(dic[solution[-1]], dic[caditade]), cid[dic[caditade]]))
                
        return sorted(g_list, key=lambda x : x[0])

    def gen_rcl(self, g_list):
        """"""
        
        rcl = []
        g_min = g_list[0][0]
        g_max = g_list[-1][0]
        
        for item in g_list:
            if round(item[0], 5) <= round((1 - self.alpha) * g_min  + self.alpha * g_max, 5):
                rcl.append(item[1])
            
            else:
                break
        
        return rcl
        
    def grasp(self):
        """
        """ 
        
        new_solution = []
        candidates = [i for i in range(self.n)]
        new_solution.append(random.choice(self.best_solution[1]))
        candidates.remove(new_solution[-1])
            
        for _ in range(1, self.n):
            g_list: list = self.gen_g(new_solution, candidates)
            rcl: list = self.gen_rcl(g_list)
            new_solution.append(random.choice(rcl))
            candidates.remove(new_solution[-1])

        return (fitness(new_solution, self.n),new_solution)
    
    def run(self):
        
        start = time.time()
        
        while self.it <= self.it_max or time.time() - start <= self.time_max:
            self.it += 1
            
            if self.verbose == True:
                if self.complex_verbose == True:
                    print(self.best_solution)
                    
                else:
                    print(self.best_solution[0])
                    
            new_solution = self.grasp()      
            
            if self.best_solution[0] > new_solution[0]:
                self.best_solution = new_solution
    
        return self.best_solution