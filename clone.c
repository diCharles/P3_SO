// clone syscall 
/** this program created several  child friends from caller of clone, the child 
also receives a value througth   pointer
 this is a modifyed example from :
 https://codesteps.com/2014/05/19/c-programming-creating-a-child-process-using-clone/
 
*/



// necesary headers
 #define _GNU_SOURCE // never forget this define 

#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <sys/wait.h>


#define N_CLONES 4U // NUMBER OF CLONES TO BE CREATED
#define ITERACIONES 2000000000

long double pi_acumm[N_CLONES] ={ 0} ;

// thread that differ from parent in execution

int child_fn(void * arg)
{
		int val = * ((int *) arg ) ;
		printf("clonated child  received %d \n", val);
		pi_acumm[val] +=1 ;
		exit(0);
}	

int main()
{
	void * child_stack[N_CLONES] = {NULL};
	pid_t pid_chids[N_CLONES]= {0};
	
	int clone_maker = 0 ;
	for(clone_maker = 0 ; clone_maker<N_CLONES; clone_maker++)
	{
		// allocate mem for clone thread
		
		child_stack[clone_maker]= malloc(1024*1024);
		//creating child with a given value
		int val_to_child = 1;
		pid_chids[clone_maker] = clone( child_fn , child_stack[clone_maker] + (1024 * 1024) , SIGCHLD | CLONE_VM , (void * ) (&val_to_child)); 
		
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
		//free clone child mem
		free(child_stack[clone_maker]);
	}
	long double pi  = pi_acumm[0] +  pi_acumm[1] + pi_acumm[2] + pi_acumm[3] ;
	printf("PI con %d iteraciones: %Lf\n", ITERACIONES, (pi*4));
	

	return 0 ; 
}
