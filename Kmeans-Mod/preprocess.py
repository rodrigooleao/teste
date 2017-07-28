#671 users
#9126 movies
import numpy as np

np.set_printoptions(threshold=np.inf)

def getPosById( movieData , id):
    for x in movieData:
        if( x[1] == id):
            return x[0]

    return -1
arq = open("datasets/ratings.csv")
arq2 = open("datasets/movies.csv")

texto = arq.readlines()
movies = arq2.readlines()

texto.remove( texto[0])
movies.remove( movies[0])

movieData = []
i = 0

for movie in movies:
    movie = movie.strip("\n\r").split(",")
    movieData.append( [ i , int(movie[0])])
    i+=1

dataPoints = np.array( [ [0 for x in range( 9126)] for x in range(671) ] )

for linha in texto:
    linha = linha.strip("\n").split(",")
    linha = linha[0:-1]
    linha = [int(float(x)) for x in linha ]

    a = linha[0] - 1
    b = linha[1]
 
    b = getPosById( movieData , b)
    dataPoints[a][b-1] = linha[2]


for user in dataPoints:
    print( user )


