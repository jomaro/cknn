#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// tamanho do vetor rotulado
int N;

// tamanho do vetor a ser predito
int M;

// quantidade de dimensões
int DIM;

// quantidade de vizinhos a serem considerados
int K;

// quantidade de rotulos diferentes (na realidade o número do maior rotulo +1)
int NS;

#define INF (1e7)
#define TNUM (4)

// matriz NxDIM com os dados rotulados
double *data;


// vetor de tamanho N com os rótulos de cada linha da tabela
int *rotulos_data;


// matriz MxDIM com os dados que devem ser preditos
double *test;


// rotulos da matriz test geradas pelo scikit-learn (para conferir o resultado)
int *rotulos_test;


// rotulos da matriz test que devem ser preenchidos na sua implementação
// da função Calc_DisS
int *rotulos_tests;


// rotulos da matriz test que devem ser preenchidos na sua implementação
// da função Calc_DisP
int *rotulos_testp;



void le_arquivo(char *name, char *predict) {
	FILE *f;
	
	int *p;
	char buffer[50];
	
	p = (int*)buffer;
	
	f = fopen(name, "rb");
	
	fread(buffer, 4, 4, f);
	
	N = p[0];
	M = p[1];
	DIM = p[2];
	NS = p[3];
	
	data = aligned_alloc(64, N*DIM*sizeof(double));
	test = aligned_alloc(64, M*DIM*sizeof(double));
	
	rotulos_data = aligned_alloc(64, N*sizeof(int));
	rotulos_test = aligned_alloc(64, M*sizeof(int));
	rotulos_tests = aligned_alloc(64, M*sizeof(int));
	rotulos_testp = aligned_alloc(64, M*sizeof(int));
	
	if (!data || !test || !rotulos_data || !rotulos_test || 
				!rotulos_tests || !rotulos_testp) {
		perror("não alocou as estruturas\n");
		exit(1);
	}
	
	fread(data, N*DIM, sizeof(double), f);
	
	//printf("%lf %lf %lf %lf\n", data[0], data[1], data[2], data[3]);
	
	fread(rotulos_data, N, sizeof(int), f);
	
	
	//printf("%d %d %d %d\n", rotulos_data[0], rotulos_data[1], 
	//                        rotulos_data[2], rotulos_data[3]);
	
	fread(test, M*DIM, sizeof(double), f);
	
	//printf("%lf %lf %lf %lf\n", test[0], test[1], test[2], test[3]);
	
	fclose(f);
	
	f = fopen(predict, "rb");
	
	fread(buffer, 4, 1, f);
	
	if (p[0] != M) {
		perror("os arquivos tem vetores de tamanhos diferentes");
		
		exit(2);
	}
	
	fread(rotulos_test, M, sizeof(int), f);
	
	fclose(f);
}


void Calc_DisS() {
	int i, j, k, aux;
	
	double dist, aux2;
	
	// lista dos k vizinhos mais proximos
	int viz[K+1];
	
	// distancias para os respectivos vizinhos do vetor acima
	double distancias[K+1];
	
	// quantidade de vizinhos já descoberto
	// 		para quando i ainda é menor k 
	int quant;
	
	int votos[NS];
		
	//printf("-  1: %d\n", K);
	
	// percorre os valores do test
	for(i = 0; i < M; i++){
		quant = 0;
		//printf("   1: %d\n", K);
	
		for(j = 0; j < K+1; j++) {
			distancias[j] = INF;
		}
	
		// percorre os valores base para comparação (data)
		for(j = 0; j < N; j++){
			dist = 0;
		
			for(k = 0; k < DIM; k++){
				aux2 = data[j*DIM+k] - test[i*DIM+k];
			
				dist += aux2 * aux2;
			}
			
			// atualiza o vetor dos K mais próximos
			if(quant == 0) {
				viz[0] = j; // j == 0
				distancias[0] = dist;
				
				quant = 1;
			} else {
				if (dist < distancias[quant-1]) {
					// o vetor de vizinhos é mantido ordenado
					// arrasta os valores pro lado pra abrir o espaço do novo
					for(k = quant-1; k >= 0 && dist < distancias[k]; k--){
						distancias[k] = distancias[k-1];
						viz[k] = viz[k-1];
					}
					
					distancias[k+1] = dist;
					viz[k+1] = j;
				}
				
				if(quant < K) quant++;
			}
		}
		
		// votação dos rótulos de vizinhos
		memset(votos, 0, sizeof(votos));
		
		
		for(j = 0; j < K; j++) {
			votos[rotulos_data[viz[j]]]++;
			//printf(" - %d %lf\n", viz[j], distancias[j]);
		}
		
		aux = 0;
		for(j = 1; j < NS; j++){
			if(votos[j] > votos[aux]){
				aux = j;
			}
		}
		
		rotulos_tests[i] = aux;
	}
}



int main(int argc, char **argv) {
	int i, missmatches = 0;
	
	if(argc != 4){
		perror("exemplo de entrada: ./knn data.knn 5 data-k5.knn");
		exit(4);
	}
	
	le_arquivo(argv[1], argv[3]);
	
	K = atoi(argv[2]);
	//printf("   X: %d %s\n", K, argv[2]);
	
	double start = omp_get_wtime(); //Calcula o tempo
		Calc_DisS();
	double end = omp_get_wtime(); //Calcula o tempo
	
	printf("sequencial: %f\n", end-start);
	
	
	for (i = 0; i < M; i++) {
		if (rotulos_test[i] != rotulos_testp[i] || rotulos_testp[i] != rotulos_tests[i]) {
			missmatches++;
			printf("%d: %d %d\n", i, rotulos_tests[i], rotulos_test[i]);
		}
	}
	
	printf("%d of %d missmatches\n", missmatches, M);
	
	
	free(data);
	free(test);
	
	free(rotulos_data);
	free(rotulos_test);
	free(rotulos_tests);
	free(rotulos_testp);
	
	return 0;
}

