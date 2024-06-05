from library import read
from ACO.basico import Basic
from ACO.optimized import Optimized
import asyncio
from itertools import product

dic, cid = read("qa194.tsp")

#a = ACO(verbose=True, ants = 10,Q=47, phi=0.3, alpha=1.5, time_max=120)
# Defina os valores que você deseja testar para cada parâmetro
ants_values = [5, 10, 15]
Q_values = [31, 37, 41, 43, 47, 53, 59]
phi_values = [0.1, 0.2, 0.3, 0.4 ,0.5]
alpha_values = [0.5, 1.0, 1.5, 2.0]
beta_values = [2, 2.5, 3.0, 3.5, 4.0, 4.5, 5]

# Crie uma lista de todas as combinações possíveis de parâmetros
param_grid = list(product(ants_values, Q_values, phi_values, alpha_values, beta_values))

# Inicialize as variáveis para armazenar o melhor resultado e os melhores parâmetros
best_score = float('-inf')
best_params = None

# Para cada combinação de parâmetros
for params in param_grid:
    # Crie uma instância do seu modelo com os parâmetros atuais
    print(params)
    a = Optimized(verbose=True, ants=params[0], Q=params[1], phi=params[2], alpha=params[3], beta=params[4])
    
    # Execute o modelo e obtenha o resultado
    res = a.run()[0]
    print("########################")
    # Se o resultado for melhor do que o melhor resultado atual, atualize o melhor resultado e os melhores parâmetros
    if res > best_score:
        best_score = res
        best_params = params

# Imprima os melhores parâmetros e o melhor resultado
print("Melhores parâmetros:", best_params)
print("Melhor resultado:", best_score)