import re
import math

dic = {}
cid = {}

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

async def fitness(p):
    """
    """
    fit = 0
    n = len(p)
    
    for i in range(n):
        fit = fit + euclidean_distance(dic[p[i-1]], dic[p[i]])
        
    return fit