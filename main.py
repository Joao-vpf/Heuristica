from library import read
from ACO.basico import ACO
import asyncio

dic, cid = read("qa194.tsp")

#a = ACO(verbose=True, Q=31, phi=0.3, alpha= 1.5, beta=5, time_max=120)
a = ACO(verbose=True, Q= 31, phi=0.01, alpha= 2, beta=5, time_max=120)

asyncio.run(a.run())