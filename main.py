from library import read
from TabuSearch.basic import Basic

dic, cid = read("dataset/qa194.tsp")

a = Basic(verbose=True, complex_verbose=True)

a.run()