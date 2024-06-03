from library import read
from ACO.basico import Basic
from ACO.optimized import Optimized
import asyncio

dic, cid = read("qa194.tsp")

#a = ACO(verbose=True, Q=31, phi=0.3, alpha= 1.5, beta=5, time_max=120)

a = Basic(verbose=True,ants = 10 , Q=100, phi=0.1, alpha= 1, beta= 3, time_max=60)
res1 = a.run()
a = Optimized(verbose=False, time_max=60)
res2 = a.run()

print("Basico: ", res1)
print("Optimized: ", res2)