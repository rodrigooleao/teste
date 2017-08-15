#943 users
#1682 movies
import random as rd

arq = open("datasets/u.item")

texto = arq.readlines()


for linha in texto:
    linha = linha.strip("\n").split("|")[0:2]
    print(linha[0])
    print(linha[1])
    

