// clone syscall 
/** this program created several  child friends from caller of clone, the child 
also receives a value througth   pointer
 this is a modifyed example from :
 https://codesteps.com/2014/05/19/c-programming-creating-a-child-process-using-clone/
 
*/



// necesary headers
 #define _GNU_SOURCE // never forget this define  for threads

#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <math.h>


#define SEGUNDOS 1000000

#define N_CLONES 4U // NUMBER OF CLONES TO BE CREATED
#define ITERACIONES 2000000000

long double pi_acumm[N_CLONES] ={ 0} ;

// thread that differ from parent in execution

int child_fn(void * arg)
{
	double dummy=1;
	int nClone = *((int *)arg);
	
	long long n=0;
	long long inicio = nClone*(ITERACIONES/N_CLONES);
	long long fin = (nClone+1)*(ITERACIONES/N_CLONES);
	
	//printf("Proceso %d desde %lld hasta %lld\n", nClone, inicio, fin);
	
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
		
		pi_acumm[nClone] = pi_acumm[nClone] + (dummy / ((2*n)+1));
		
	}
	exit(0);
}	

int main()
{
	struct timeval ts;
	long long start_ts;
	long long stop_ts;
	long elapsed_time;

		
	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec*1000000 + ts.tv_usec; // Tiempo inicial
	
	/**  block for clone child creation **********************/
	void * child_stack[N_CLONES] = {NULL};
	pid_t pid_chids[N_CLONES]= {0};
	int clone_nmbr[N_CLONES] = {0};
	
	int clone_maker = 0 ;
	for(clone_maker = 0 ; clone_maker<N_CLONES; clone_maker++)
	{
		// allocate mem for clone thread
		
		child_stack[clone_maker]= malloc(1024*1024);
		//creating child with a given value
		clone_nmbr[clone_maker]= clone_maker;
		pid_chids[clone_maker] = clone( child_fn , child_stack[clone_maker] + (1024 * 1024) , SIGCHLD | CLONE_VM , (void * ) (&clone_nmbr[clone_maker])); 
		
		/** check if any of previous proccess went wrong*/
		if(  (NULL== child_stack[clone_maker])  ||  (0 > pid_chids[clone_maker])   )
		{
			printf(" fatal error could not create all child clones\n");
			// free clone child mem
			return 0;
		}
		
	}
	
	// wait until  clone childs finished
	for(clone_maker = 0 ; clone_maker<N_CLONES; clone_maker++)
	{
		wait(NULL);
	}
	/** watch out free of memory*/
	for(clone_maker = 0 ; clone_maker<N_CLONES; clone_maker++)
	{
		free(child_stack[clone_maker]);
	}
	
	/** entrega de calculos y tiempos */
	long double pi  = pi_acumm[0] +  pi_acumm[1] + pi_acumm[2] + pi_acumm[3] ;
	printf("PI con %d iteraciones: %Lf\n", ITERACIONES, (pi*4));
	
	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec*1000000 + ts.tv_usec; // Tiempo final
	elapsed_time = (int) (stop_ts - start_ts); //Tiempo transcurrido
	
	printf("Completado en %ld segundos\n",(elapsed_time/SEGUNDOS));
	

	return 0 ; 
}
