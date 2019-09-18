
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>

#define ITERACIONES 2000000000
#define CUADRUPLE 4
#define SEGUNDOS 1000000
#define NTHREADS 3



/** father global varaibles can be shared between threads*/
long double pi_acumm[NTHREADS] ={ 0} ;

void *serie(void *num_thread)
{
	double dummy=1;
	int nthread = *((int *)num_thread);
	long long n=0;
	long long inicio = nthread*(ITERACIONES/NTHREADS);
	long long fin = (nthread+1)*(ITERACIONES/NTHREADS);
	

	printf("Proceso %d desde %lld hasta %lld\n", nthread, inicio, fin);
	
	//Serie de PI
	for(n=inicio; n<fin; n++)
	{
		if((n%2)==0) //Par
		{
			dummy = -1;
		}
			else		//Impar
		{
			dummy = 1;
		}
		
		pi_acumm[nthread] = pi_acumm[nthread] + (dummy / ((2*n)+1));
	}

}

int main(void)
{
	struct timeval ts;
	long long start_ts;
	long long stop_ts;
	long elapsed_time;
	int threadArr[NTHREADS];
	pthread_t tid[NTHREADS];
		
	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec*1000000 + ts.tv_usec; // Tiempo inicial
	
	
	//Crea los hilos
	for(int i=0; i<NTHREADS; i++)
	{
		
		threadArr[i] = i;
		
		pthread_create(&tid[i], NULL, serie, (void*) &threadArr[i]);
	}
	
	//Integra los hilos
	for(int x=0; x<NTHREADS; x++)
	{
		pthread_join(tid[x], NULL);	
		printf("Integracion de proceso %d\n", x);
	}
	
	//Impresion final
	long double pi  = pi_acumm[0] +  pi_acumm[1] + pi_acumm[2] + pi_acumm[3] ;
	printf("PI con %d iteraciones: %Lf\n", ITERACIONES, (pi*CUADRUPLE));
	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec*1000000 + ts.tv_usec; // Tiempo final
	elapsed_time = (int) (stop_ts - start_ts); //Tiempo transcurrido
	
	
	printf("Completado en %ld segundos\n",(elapsed_time/SEGUNDOS));
	
	
	return 0;	
}