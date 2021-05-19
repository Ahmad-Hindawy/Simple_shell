#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>



// Executes the command given in an array of strings
void execute(char **commands){
	int ret;
	//forking
	pid_t pid = fork();
	if (pid == -1){		//failed
		printf("\nProccess forking failed!");
	}
	else if (pid == 0){	//child proccess
		ret = execvp(commands[0],commands);
		if (ret < 0) {	//execusion failed
			printf("\nCommand execusion failed!");
		}
		exit(0);
	}
	else{			//parent section
		/* The parent should wait for the child proccess to terminate or not.
		 * This is determined by the user providing & as a last parameter or not.
		 * if given then the parent will NOT wait for the child to terminate and
		 * vice versa*/
		// TODO:handle the & parameter

		wait(NULL); 	//waits for the chiled to terminate
		//TODO:what would happen in case of multiple childs?

		return;
	}	
}


int main() {
	


	while(1){
		
	}

}
