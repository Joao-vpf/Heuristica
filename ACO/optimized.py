from library import euclidean_distance, dic, cid, fitness
from optimizers import opt2_s, opt3_s
import random
import time
from multiprocessing import Process, Manager
import math
import copy

class Optimized:
    """
        phero = pheromone matrix
    """
    n = 0
    phero = {}
    probabilities = {}
    ants  = 5
    it_max = None
    time_max = 180
    phi = 0.01
    alpha = 2
    beta = 5
    Q = 31
    verbose = False
    complex_verbose = False
    it_opt = 200
    optimizers = [opt2_s, opt3_s]
    multiprocess = 0
    
    def init_matrix(self):
        """
            objective:
                initialize pheromone matrix
        """
        global dic
        n = len(dic)
        
        for keys in dic.keys():
            self.phero[keys] = {}
            
            for keys2 in range(n):
                if keys == keys2: 
                    continue
                
                self.phero[keys][keys2] = 1.0
    
    def __init__(self, ants: int = 5, it_max: int = None, it_opt: int = 200, time_max: float  = 180, phi: float = 0.01, alpha: float = 2, beta: float = 5, Q: int = 31, verbose: bool = False, complex_verbose: bool = False, optimizers: list = [opt2_s, opt3_s], multiprocess = 0) -> None:
        """
            objective:
                initialize parameters
                
            Parameters:
                ants: int = number of ants
                it_max: int = number of iterations 
                time_max: float  = maximum time in seconds
                phi: float = evaporation rate
                alpha: float = influence of pheromone
                beta: float = influence of visibility
                Q: int = constant to evaporate
        """ 
    
        if(len(dic) == 0 or len(cid) == 0):
            return None
        
        self.n = len(dic)
        self.ants = ants
        self.it_max = it_max
        self.it_opt = it_opt
        self.time_max = time_max
        self.phi = phi
        self.alpha = alpha
        self.beta = beta
        self.Q = Q
        self.verbose = verbose
        self.complex_verbose = complex_verbose
        self.optimizers = optimizers
        self.multiprocess = multiprocess
        self.init_matrix()
    
    def fxy(self, _from , _to , k, probabilities):
        """
        """
        global dic
        
        visibility = (1/euclidean_distance(dic[_from], dic[_to])) ** self.beta
        probabilities[_to] = (self.phero[_from][_to] ** self.alpha) * visibility
        probabilities[_to] /= (k ** self.alpha) * visibility
    
    def fk(self, _from, k):
        """
        """
        global dic
        
        sum_t = 0
        
        for not_vis in k:
            sum_t += self.phero[_from][not_vis] ** self.alpha
        
        return sum_t
    
    def roullete(self, probabilities):
        """
        """
        
        sum_t = sum(probabilities.values())
        probabilities_norm = {}
        
        for idx, prob in probabilities.items():
            probabilities_norm[idx] = prob / sum_t
        
        roll = random.uniform(0.0, 1.0)
        sum_t = 0
        
        for idx, prob in probabilities_norm.items():
            sum_t  = sum_t + prob
            if sum_t >= roll:
                return idx
            
        return list(probabilities_norm.keys())[-1]
    
    def optimizations(self, new_path, fit):
        
        aux = copy.deepcopy(new_path)
        aux_fit = fit
        
        for opt in self.optimizers:
            for _ in range(self.it_opt):
                aux = opt(new_path, self.n)
                aux_fit = fitness(aux, self.n)
                
                if aux_fit < fit:
                    new_path = copy.deepcopy(aux)
                    fit = aux_fit

        return new_path, fit               
    
    def new_path(self, idx: int, list_ants_path: list):
        """
        """
        global dic
        
        new_path = []
        list_not_visited = [i for i in range(self.n)] 
        point = random.randrange(0, self.n)
        new_path.append(point)
        list_not_visited.remove(point)
        
        while len(list_not_visited):
            probabilities = {}
            k = self.fk(point, list_not_visited)
            
            for i in range(self.n):
                if not i in list_not_visited:
                    continue
                
                self.fxy(point, i, k, probabilities)
                
            point = self.roullete(probabilities)
            new_path.append(point)
            list_not_visited.remove(point)
        
        fit = fitness(new_path, self.n)
        new_path, fit = self.optimizations(new_path, fit)
        list_ants_path[idx] = (fit, new_path)

    def evapore(self):
        """
        """
        
        for keys in self.phero.keys():
            for keys2 in range(self.n):
                if keys == keys2: 
                    continue 
    
                self.phero[keys][keys2] = max((1 - self.phi) * self.phero[keys][keys2], 1e-5)
    
    def att_phero(self, idx: int, list_ants_path: list):
        """
        """
        
        for i in range(self.n):
            self.phero[list_ants_path[idx][1][i-1]][list_ants_path[idx][1][i]] = min(self.phero[list_ants_path[idx][1][i-1]][list_ants_path[idx][1][i]] + self.Q / list_ants_path[idx][0], \
                1.0)
     
    def init_all_paths(self):
        """
        """
        
        list_ants_path = [[] for _ in range(self.ants)]
        
        if self.multiprocess:
            
            maneger = Manager()
            list_ants_path = maneger.list(list_ants_path)
            
            process = []
            for idx in range(self.ants):
                pro = Process(target = self.new_path, args = (idx, list_ants_path)) 
                process.append(pro)
                pro.start()
            
            for pro in process:
                pro.join()
            
        else:
            for idx in range(self.ants):
                self.new_path(idx, list_ants_path)
            
        return list(list_ants_path)
           
    def run(self):
        """
            objective:
                Activate the algorithm
        """
        
        if self.time_max is None and self.it_max is None:
            self.time_max = 60
        
        start_time = time.time()
        it =  0
        result = [math.inf, []]
        
        while True:
            if not self.time_max is None and self.time_max < time.time() - start_time:
                 break
             
            if not self.it_max is None and self.it_max < it:
                break
            
            list_ants_path = self.init_all_paths()
            
            self.evapore()
            
            if result[0] != math.inf:
                list_ants_path.append(result) 
                self.att_phero(self.ants, list_ants_path)
                
            for idx in range(self.ants):
                self.att_phero(idx, list_ants_path)
            
            it  = it + 1 
              
            for idx in range(self.ants):
                item = list_ants_path[idx]
                
                if result[0] > item[0]:
                    result = item
                    
                if self.complex_verbose:
                    print(item)
                
            if self.verbose:
                if self.complex_verbose:
                    print(result)
                
                else:
                    print(result[0])   
                
        return result
    