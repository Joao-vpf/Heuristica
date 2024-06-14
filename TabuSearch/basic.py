from library import fitness, create_path
import random
import copy
import time

class Basic:
    
    it = 0
    it_max = 0
    time_max = 180
    rm_it_tabu_list = 5
    alpha = 3
    beta = 60
    tabu_list = {}
    best_solution = None
    verbose = False
    complex_verbose = False
    n = 0
    
    def __init__(self, path_init = None, it_max = 0, time_max = 180, verbose = False, complex_verbose = False, alpha = 10, beta = 60) -> None:
        if path_init is None:
            path_init = create_path()
            
        self.it_max = it_max
        self.time_max = time_max
        self.verbose = verbose
        self.beta = beta
        self.complex_verbose = complex_verbose
        self.n = len(path_init)
        self.alpha = min(max(self.alpha, alpha), self.n)
        self.best_solution = (fitness(path_init, self.n), path_init)
        self.att_tabu_list(path_init)
        
    def att_tabu_list(self, path):
        """
        """
        
        if len(self.tabu_list) == 0:
            for i in range(self.n):
                self.tabu_list[path[i-1]] = {path[i]: self.it}
            
            pass
        
        for i in range(self.n):
            self.tabu_list[path[i-1]][path[i]] = self.it
            
        remove_list = {}
        for key1, item in self.tabu_list.items():
            remove_list[key1] = []
            for key, element in item.items():
                if self.it - element > self.rm_it_tabu_list:
                    remove_list[key1].append(key)
    
        for key, item in remove_list.items():
            for idx in item:
                self.tabu_list[key].pop(idx)

    def neighbor(self):
        """
        """
        new_path = copy.deepcopy(self.best_solution[1])
        it = 0
        it_beta = 0
        
        while it < self.alpha and it_beta  < self.beta:
            it_beta += 1
            l = random.randint(0, self.n-1)
            r = random.randint(0, self.n-1)
            
            while l == r:
                l = random.randint(0, self.n-1)
                
            if  (not new_path[l] in self.tabu_list[new_path[r-1]] or not new_path[r] in self.tabu_list[new_path[l-1]]):
                new_path[l], new_path[r] = new_path[r], new_path[l]
                
                it += 1
        
        return (fitness(new_path, self.n),new_path)
    
    def run(self):
        
        start = time.time()
        while self.it <= self.it_max or time.time() - start <= self.time_max:
            self.it += 1
            
            if self.verbose == True and self.it%10000 == 1:
                if self.complex_verbose == True:
                    print(self.best_solution)
                    
                else:
                    print(self.best_solution[0])
                    
            new_solution = self.neighbor()
            
            if new_solution[0] < self.best_solution[0]:
                self.best_solution = new_solution
            
            
            self.att_tabu_list(self.best_solution[1])
            

    