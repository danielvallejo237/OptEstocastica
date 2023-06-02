import numpy as np
from scipy.linalg import sqrtm, eig

Ngen = 500
dimension = 10
r_inicial = -5
r_final = 5

def Ackley():
    

problema = 'ackley'
# problema = 'esfera'
# problema = 'griewank'

lambda_ = dimension ** 2  # hijos
mu = np.ceil(lambda_ / 4)  # padres

# colocar media de la distribucion de manera aleatoria
y_inicial = r_inicial + (r_final - r_inicial) * np.random.rand(dimension)
C = np.eye(dimension)  # unos en la diagonal, ceros en otra parte
sigma_inicial = 1

# funciones con minimo en ZERO
minimo_conocido = np.zeros(dimension)
fmin_convergencia = eval(f"{problema}(minimo_conocido)")

# function
tc = 1 + (dimension * (dimension + 1)) / (2 * mu)
tau = (1.0 / np.sqrt(2 * dimension)) / 8

y_l = np.zeros((lambda_, dimension))
fitness = np.zeros(lambda_)
y = y_inicial
sigma_l = np.ones(lambda_) * sigma_inicial
s_l = np.zeros((lambda_, dimension))
z_l = np.zeros((lambda_, dimension))
index = np.arange(mu)

print('iniciando el algoritmo..')

for i in range(Ngen):
    B, L = eig(C)  # eigenvectors B, eigenvalues C
    sigma_mean_best = np.mean(sigma_l[index[:mu]])
    sigma_l = sigma_mean_best * np.exp(tau * np.random.lognormal(0, 1, (lambda_, 1)))  # mutaciones LOGNORMAL segun paper!!
    # -----------Generar poblacion----------%
    C_2 = B @ sqrtm(L) @ B.T  # raiz cuadrada de los eigenvalues
    for j in range(lambda_):
        s_l[j, :] = C_2 @ np.random.randn(dimension)  # tambien se puede hacer C_2= Cholesky(C)
        z_l[j, :] = sigma_l[j] * s_l[j, :]
        y_l[j, :] = y + z_l[j, :]
        fitness[j] = eval(f"{problema}(y_l[j, :])")

    # guardamos el indice del ordenamiento
    index = np.argsort(fitness)
    fitness_sorted = fitness[index]

    # actualizar el centro con los mu mejores.
    y = y + np.mean(z_l[index[:mu], :], axis=0)

    # actualizar la matriz de covarianza con rank1
    sst = np.zeros((dimension, dimension))
    for imu in range(mu):
        sst += np.outer(s_l[index[imu], :], s_l[index[imu], :])
    sst /= mu

    C = (1 - 1 / tc) * C + (1 / tc) * sst

# Ngen
np.set_printoptions(precision=16, suppress=True)
print('Centro (media)  de la distribucion ')
print(y)
print('Aptitud')
print(eval(f"{problema}(y)"))