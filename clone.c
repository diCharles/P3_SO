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

#define N_CLONES 8U // NUMBER OF CLONES TO BE CREATED

// thread that differ from parent
int child_fn(void * arg)
{
		int val = * ((int *) arg ) ;
		printf("clonated child  received %d \n", val);
}	

int main()
{
	// allocate mem for clone thread
	void * child_stack = malloc(1024*1024);

	// signal any error in allocation
		if ( NULL== child_stack)
		{	
			printf("unable to allocate mem for child \n");		
		}
		else
		{
			//passing value to child
			int val_to_child = 4;
			
			int pid_child = clone( child_fn , child_stack + (1024 * 1024) , SIGCHLD, (void * ) (&val_to_child)); 
			if( 0 > pid_child)
			{
				printf("unable to clone a child \n");
			}	
			else
			{
				
				//able to clone a child
				wait(NULL) ; // better wait his pid
				free(child_stack);
			}
		}

	return 0 ; 
}
