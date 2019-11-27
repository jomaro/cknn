

all: knn knn_optim knn_line knn_line_optim

knn: knn.c
	gcc -o knn -Wall -fopenmp knn.c

knn_line: knn_line.c
	gcc -o knn_line -Wall -fopenmp knn_line.c
	
	
knn_line_optim: knn_line.c
	gcc -o knn_line_optim -O3 -mavx -march=native -Wall -fopenmp knn_line.c

knn_optim: knn.c
	gcc -o knn_optim -O3 -mavx -march=native -Wall -fopenmp knn.c

