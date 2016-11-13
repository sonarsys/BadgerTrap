#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <libgen.h>

int main (int argc, char* argv[])
{
	pid_t pid;
	int status, ret=0;
        char *process_names[1];
	
	if(argc < 2)
	{
		printf("Usage: ./badger-trap <name | pid | command | help> {arguments}\n");
		printf("Example1: ./badger-trap command ls\n");
		printf("Example2: ./badger-trap command ./a.out <arguments>\n");
		printf("Example3: ./badger-trap name blacksholes omp-csr gem5\n");
		printf("Example3: ./badger-trap pid 1124 2346 11\n");
		return 0;
	}

	if(strcmp(argv[1],"command")==0)
	{
		if(argc < 3)
		{
			printf("Please provide a command to run\n");
			printf("Example1: ./badger-trap command ls\n");
			printf("Example2: ./badger-trap command ./a.out <arguments>\n");
			return 0;
		}
	        process_names[0] = basename(argv[2]);
	        ret = syscall(314,process_names,1,1);
	        switch ((pid = fork()))
	        {
	        case -1:
	                perror ("Fork Failed !!");
	                break;
	        case 0:
	                execvp(argv[2], &argv[2]);
	                exit(0);
	                break;
	        default:
	                printf("Badger Trap launched with process %s\n",argv[2]);
	                wait(&status);
	                break;
	        }
	        ret = syscall(314,NULL,0,1);
	}
	else if (strcmp(argv[1],"name")==0)
	{
		ret = syscall(314,&argv[2],argc-2,1);
	}
	else if (strcmp(argv[1],"pid")==0)
	{
		ret = syscall(314,&argv[2],argc-2,-1);
	}
	else if((strcmp(argv[1],"help")==0))
	{
		printf("Usage: ./badger-trap <names | pid | command | help> {arguments}\n");
                printf("Example1: ./badger-trap command ls\n");
                printf("Example2: ./badger-trap command ./a.out <arguments>\n");
                printf("Example3: ./badger-trap names blacksholes omp-csr gem5\n");
                printf("Example3: ./badger-trap pid 1124 2346 11\n");
                return 0;
	}
	else
	{
		printf("Cannot run command provided. Run ./badger_trap help for more information\n");
		return 0;
	}
        return ret;
}
