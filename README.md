

generate
========

Gera um conjunto de dados para treino, esse arquivo não gera uma predição 
baseado no K.

```
./generate N M D C NAME
./generate 10000 1500 60 10 test1.knn
```

onde:
 - **N** é o tamanho da matriz de treino
 - **M** é o tamanho da matriz de testes
 - **D** é quantidade de dimensões nessas matrizes (uma NxD e outra MxD)
 - **C** é a quantidade de classes diferentes
 - **NAME** é um nome pro arquivo de saida


predict
=======

Gera um arquivo com as predições de um conjunto de testes.

```
./predict K NAME OUT
./predict 5 test1.knn test1.out5.knn
```

onde:
 - **K** é o tamanho da matriz de treino
 - **NAME** é um nome do arquivo gerado pelo generate
 - **OUT** é um nome do arquivo de saida para a predição do conjunto de dados 
 			**NAME** com a quantidade de vizinhos **K**


knn
===


Depois de compilado testar como:

```
./knn NAME K OUT
./knn test1.knn 5 test1.out5.knn
```


um fluxo completo:


```
./generate 10000 1500 60 10 test1.knn
./predict 5 test1.knn test1.out5.knn
make
./knn test1.knn 5 test1.out5.knn
```


