
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

#define ITERACIONES 2000000000
#define CUADRUPLE 4
#define SEGUNDOS 1000

int main(void)
{
	struct timeval ts;
	long long start_ts;
	long long stop_ts;
	long elapsed_time;
	double pi = 0, dummy = 0;
	
	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial
	
	//Serie de PI
	for(long long n=0; n<ITERACIONES; n++)
	{
		if((n & 0b1 ) == 0) //Par
			dummy = -1;
		else		//Impar
			dummy = 1;
		pi += dummy / ((2*n)+1);
	}
	
	printf("PI con %d iteraciones: %f\n", ITERACIONES, (pi*4));
	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final
	elapsed_time = (int) (stop_ts - start_ts); //Tienpo transcurrido
	printf("Completado en %ld segundos\n",(elapsed_time/1000000));
	
	return 0;	
}