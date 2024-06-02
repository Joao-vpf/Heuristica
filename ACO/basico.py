from library import euclidean_distance, dic, cid, fitness
import concurrent.futures
import asyncio
import random
import time
import json
import math


class ACO:
    """
        phero = pheromone matrix
    """
    phero = {}
    probabilities = {}
    ants  = 5
    it_max = None
    time_max = 60
    phi = 0.3
    alpha = 0.5
    beta = 0.5
    Q = 5
    verbose = False
    complex_verbose = False
    
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
                
                self.phero[keys][keys2] = random.uniform(1.0, min(self.Q, 2))
    
    def __init__(self, ants: int = 5, it_max: int = None, time_max: float  = 60, phi: float = 0.3, alpha: float = 0.5, beta: float = 0.5, Q: int = 5, verbose: bool = False, complex_verbose: bool = False) -> None:
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
        
        self.anst = ants
        self.it_max = it_max
        self.time_max = time_max
        self.phi = phi
        self.alpha = alpha
        self.beta = beta
        self.Q = Q
        self.verbose = verbose
        self.complex_verbose = complex_verbose
        self.init_matrix()
    
    async def fxy(self, _from , _to , k):
        """
        """
        global dic
        
        visibility = math.pow(1/euclidean_distance(dic[_from], dic[_to]), self.beta)
        self.probabilities[_to] = math.pow(self.phero[_from][_to], self.alpha) * visibility
        self.probabilities[_to] = self.probabilities[_to]/ (math.pow(k, self.alpha) * visibility)
        self.probabilities[_to] = self.probabilities[_to]
    
    async def fk(self, _from, k):
        """
        """
        global dic
        
        sum_t = 0
        for not_vis in k:
            sum_t = sum_t + euclidean_distance(dic[_from], dic[not_vis])
        
        return sum_t
    
    async def roullete(self):
        """
        """
        
        sum_t = sum(self.probabilities.values())
        probabilities_norm = {}
        
        for idx, prob in self.probabilities.items():
            probabilities_norm[idx] = prob / sum_t
        
        roll = random.uniform(0.0, 1.0)
        sum_t = 0
        
        for idx, prob in probabilities_norm.items():
            sum_t  = sum_t + prob
            if sum_t >= roll:
                return idx
            
        return list(probabilities_norm.keys())[-1]
    
    async def new_path(self, idx: int, list_ants_path: list):
        """
        """
        global dic
        
        n = len(dic) 
        new_path = []
        list_not_visited = [i for i in range(n)] 
        point = random.randrange(0, n)
        new_path.append(point)
        list_not_visited.remove(point)
        
        while len(list_not_visited):
            self.probabilities = {}
            k = await self.fk(point, list_not_visited)
            
            for i in range(n):
                if not i in list_not_visited:
                    continue
                
                await self.fxy(point, i, k)
                
            point = await self.roullete()
            new_path.append(point)
            list_not_visited.remove(point)
        
        fit = await fitness(new_path)
        list_ants_path[idx] = (fit, new_path)

    async def evapore(self):
        """
        """
        
        n = len(self.phero)
        
        for keys in self.phero.keys():
            for keys2 in range(n):
                if keys == keys2: 
                    continue 
    
                self.phero[keys][keys2] = max((1 - self.phi) * self.phero[keys][keys2], 1e-10)
    
    async def att_phero(self, idx: int, list_ants_path: list):
        """
        """
        
        n = len(list_ants_path[idx])
        
        for i in range(n):
            self.phero[list_ants_path[idx][1][i-1]][list_ants_path[idx][1][i]] = self.phero[list_ants_path[idx][1][i-1]][list_ants_path[idx][1][i]] + self.Q / list_ants_path[idx][0]

    async def run(self):
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
            
            tasks = []
            list_ants_path = [[] for _ in range(self.ants)]
            
            for idx in range(self.ants):
                tasks.append(asyncio.create_task(self.new_path(idx, list_ants_path)))
            
            await self.multiprocess(tasks)
            tasks = []
            
            await self.evapore()
            
            if len(result[1]):
                list_ants_path.append(result)
                tasks.append(asyncio.create_task(self.att_phero(self.ants, list_ants_path)))
            
            for idx in range(self.ants):
                tasks.append(asyncio.create_task(self.att_phero(idx, list_ants_path)))
                
            await self.multiprocess(tasks)
            
            it  = it + 1 
              
            for idx in range(self.ants):
                item = list_ants_path[idx]
                
                if result[0] > item[0]:
                    result = item
                    
                if self.complex_verbose and it%100 == 0:
                    print(item)
                
            if self.verbose and it%100 == 0:
                print(result)
                
        return result
            
        
    async def multiprocess(self, tasks):
        """
        """
        
        loop = asyncio.get_event_loop()
        
        with concurrent.futures.ThreadPoolExecutor() as executor:
            loop_tasks = [loop.run_in_executor(executor, asyncio.ensure_future, task) for task in tasks]
            await asyncio.gather(*loop_tasks)
    