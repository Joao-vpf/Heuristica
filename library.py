import re
import math
import random

dic: dict = {}
cid: dict = {}

def euclidean_distance(coord1, coord2):
    x1, y1 = coord1
    x2, y2 = coord2
    
    return math.sqrt((x2 - x1)**2 + (y2 - y1)**2)

def read(filename):
    global dic, cid
    
    coord_pattern = re.compile(r"(\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)")
    
    with open(filename, 'r') as file:
        data = file.read()
        
        matches = coord_pattern.findall(data)
        for match in matches:
            node = int(match[0])-1
            x_coord = float(match[1])
            y_coord = float(match[2])
            dic[node] = (x_coord, y_coord)
            cid[(x_coord, y_coord)] = node

    return dic, cid

def fitness(p, n, r: int = None):
    """

        Parameters:
            p: Path
            n: Size path
    """
    fit = 0
    
    for i in range(n):
        fit = fit + euclidean_distance(dic[p[i-1]], dic[p[i]])
        
    return fit

def create_path():
    new_path = list(dic.keys())
    random.shuffle(new_path)
    
    return new_path