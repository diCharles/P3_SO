//
/**
Referencias de codigos ejemplo
http://users.cs.cf.ac.uk/Dave.Marshall/C/node27.html
https://sites.google.com/site/sanselgroup/unix/shared-memory
https://stackoverflow.com/questions/8186953/shared-memory-with-two-processes-in-c

https://en.wikipedia.org/wiki/File_system_permissions explains the 0777
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
	/** for elapsed time of program */
	struct timeval ts;
	long long start_ts;
	long long stop_ts;
	long elapsed_time;
	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec*1000000 + ts.tv_usec; // Tiempo inicial
	
	
	//*******  get and attach a  shared memory segment ********************************************************
	
	//                                                create segment if already does not exist in kernel or ed with permitions for R/W for all
    int key = shmget(0x0040000,sizeof( double), 0777 | IPC_CREAT );
    // Shmget validation.
    if( 0 >= key)
    {
        printf("Key generation error.\n");
        exit(-1);
    }
	
	 // Shared varable: between childs and parent
    long double * pi = 0 ;
	
	//validate mem attachment, if success a mem link is created
     pi = shmat(key,NULL,0);
    if( -1 == (*pi) )
    {
        printf("Shared memory allocation error.\n");
        exit(-1);
    }

	
	//*******  get and attach a  shared memory segment ********************************************************
	
   //********  calculate pi ***********************************************************************************
    *pi = 0;// shared varaible init
	
	// fork specifyed N_childs
    uint8_t child_maker = 0;
    for(child_maker = 0; child_maker < N_CHILDS; child_maker++)
    {
        pid_t  pid_tmp = fork();//child is forked
        if( 0 == pid_tmp )
        {	
            *pi += child_fn(child_maker);
            exit(0);// child terminates
        }
    }
	
	//before print wait child termination
	for(child_maker = 0; child_maker < N_CHILDS; child_maker++)
    	wait(NULL);
	
	printf("PI con %d iteraciones: %Lf\n", ITERACIONES, (*pi)*4);
	//********  calculate pi ***********************************************************************************
	
	/** print elapsed time of the program */
	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec*1000000 + ts.tv_usec; // Tiempo final
	elapsed_time = (int) (stop_ts - start_ts); //Tiempo transcurrido
	printf("Completado en %ld segundos\n",(elapsed_time/SEGUNDOS));
	
	//destroy mem link with shmdt.
	
  return ( 0);
}

/** this function will be called by childs  and will calculate 
	and return a specific section of leibniz series				*/
double child_fn(int identifier)
{
	 
	double dummy=1;
	int nchild = identifier;
	
	long long n=0;
	long long inicio = nchild*(ITERACIONES/N_CHILDS);
	long long fin = (nchild+1)*(ITERACIONES/N_CHILDS);
	
	//printf("Proceso %d desde %lld hasta %lld\n", nchild, inicio, fin);
	long double  pi_accum = 0;
	
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
	
	return ( pi_accum ) ;
}