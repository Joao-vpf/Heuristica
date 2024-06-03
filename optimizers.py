import random
import copy

def opt2_s(p, n):
    """
    """
    
    aux = copy.deepcopy(p)
    l = random.randint(0, n-1)
    r = random.randint(0, n-1)
    
    while l == r:
        l = random.randint(0, n-1)
        
    if l > r:
        l, r = r, l
        
    while l > r:
        aux[l], aux[r] = aux[r], aux[l]
        l += 1
        r -= 1
        
    return aux

def opt3_s(p, n):
    """
    """
    
    aux = copy.deepcopy(p)
    l = random.randint(0, n-1)
    r = random.randint(0, n-1)
    
    while l == r:
        l = random.randint(0, n-1)
        
    if l > r:
        l, r = r, l 
        
    if l + r > n:
        return aux
    
    start_idx = random.randint(0, n - l - r)

    aux[start_idx:start_idx + l], aux[start_idx + l:start_idx + l + r] = aux[start_idx + l:start_idx + l + r], aux[start_idx:start_idx + l]

    return aux