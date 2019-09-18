//
/**
Referencias de codigos ejemplo
http://users.cs.cf.ac.uk/Dave.Marshall/C/node27.html
https://sites.google.com/site/sanselgroup/unix/shared-memory
*/
//libraries for shared mem segments
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/sysinfo.h>

/** libraries for shared mem*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/wait.h>

#define SEGUNDOS 1000000

#define N_CHILDS 4U // NUMBER OF CLONES TO BE CREATED
#define ITERACIONES 2000000000

// shemget get a shared memory segment
// shemat attach/dettach a shared memory segment
double child_fn(int identifier); 

int main()
{
	
	// set shared memory segment
	
	//                                                create segment if already does not exist in kernel
    int key = shmget(0x0040000,sizeof( double), 0777 | IPC_CREAT );
    // Shmget validation.
    if( 0 >= key)
    {
        printf("Key generation error.\n");
        exit(-1);
    }
	
	 // Shared varable, with this pointer childs will share context with brother and parent
    long double * pi = shmat(key,NULL,0);
    // Shmget validation.
    if( -1 == (*pi) )
    {
        printf("Shared memory allocation error.\n");
        exit(-1);
    }
    // Initialize shared variable in zero.
    *pi = 0;
	
	// fork specifyed children
	pid_t pid_tmp;
    // Loop counter.
    uint8_t child_maker = 0;
    // Process creation.    
    for(child_maker = 0; child_maker < N_CHILDS; child_maker++)
    {
        pid_tmp = fork();
        if( 0 == pid_tmp )
        {
			// here a child is forked
            *pi += child_fn(child_maker);
			//here a child terminates
            exit(0);
        }
    }
	
	for(child_maker = 0; child_maker < N_CHILDS; child_maker++)
    	wait(NULL);
	
	printf("PI con %d iteraciones: %Lf\n", ITERACIONES, (*pi)*4);
	
	
  return ( 0);
}

/** this function will be called */
double child_fn(int identifier)
{
	//calculate and return a specific section of leibniz series 
	double dummy=1;
	int nchild = identifier;
	
	long long n=0;
	long long inicio = nchild*(ITERACIONES/N_CHILDS);
	long long fin = (nchild+1)*(ITERACIONES/N_CHILDS);
	
	//printf("Proceso %d desde %lld hasta %lld\n", nchild, inicio, fin);
	long double  pi_accum = 1;
	/***
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
		
		pi_accum = pi_accum + (dummy / ((2*n)+1));	
	}
	**/
	return ( pi_accum ) ;
}