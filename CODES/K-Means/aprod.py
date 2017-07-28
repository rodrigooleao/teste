from matplotlib.pyplot import*
import numpy as np
import matplotlib.pyplot as plt
import random as rd
import math

maxIter = 200
N = 1000
inter = 700000

S = np.zeros( (  N ,  N))
R = np.zeros( (  N ,  N))
A = np.zeros( (  N ,  N))

def getInput( qtde ):
	colors = ['b' , 'g' , 'r' , 'c']
	return [( rd.randint(-inter , inter) , rd.randint(-inter , inter) , rd.choice( colors)) for i in range(qtde)]

def plotResult( vet ):
	axvline(x=0,color='k')
	axhline(y=0,color='k')

	colors = np.array(['b' , 'g' , 'r' , 'c' , 'm' , 'y' , 'k' , 'w'])
	for (x,y,z) in vet:
		x = int(x)
		y = int( y )
		z = int (z)
		plot([x], [y], marker='o', markersize=5, color= colors[z % colors.shape[0]])

	show()

def NegSquared(a , b):
	return - math.sqrt( (int(a[0]) - int(b[0])) ** 2 + (int(a[1]) - int(b[1])) ** 2)


points = np.array( getInput( N ))

arq = open("results.txt")

texto = arq.readlines()

i = 0
for linha in texto:
	linha = linha.split()

	a = int( linha[0])
	b = int( linha[1])
	c = int( linha[2])

	points[i] = (a , b , c)

	i +=1

plotResult( points)
