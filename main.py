from library import read
from ACO.basico import ACO
import asyncio

dic, cid = read("wi29.tsp")

a = ACO(verbose=True, Q=100, alpha=2, beta=5, time_max=120)

asyncio.run(a.run())