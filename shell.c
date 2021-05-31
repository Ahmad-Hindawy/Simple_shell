#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

//these libraries are used to read line from stdin and also having a history of commands
#include<readline/readline.h>
#include<readline/history.h>


#define MAX_BUF_SIZE 64 //maximum number of characters for any buffer
#define DELIM " \t\r\n\a" //escape characters that seperates tokens
int flag = 0 ;

char** parse_command(char *str ){
	int buf_size = MAX_BUF_SIZE , pos = 0;
	char **tokens_arr = malloc(buf_size * sizeof(char*));
	char *token;

	if(!tokens_arr){		//The malloc function did not complete
		printf("\nAllocation error!");
		exit(0);
	}
	token = strtok(str,DELIM);
	while(token != NULL){
		tokens_arr[pos] = token;
		pos++;

		if(pos >= buf_size){
			buf_size += MAX_BUF_SIZE;
			tokens_arr = realloc(tokens_arr , buf_size * sizeof(char*));
			if(!tokens_arr){		//The malloc function did not complete
				printf("\nAllocation error!");
				exit(0);
			}
		}

		token = strtok(NULL,DELIM);
	}
	tokens_arr[pos] = NULL;
	//*position_var = pos; //number of tokens + 1 for null termination
	//printf("%d\n",pos);
	return tokens_arr;

}
int get_command(char *str){
	char *buf;
	buf = readline("\n>>>");
	if (strlen(buf) != 0){
		add_history(buf);
		strcpy(str,buf);
		return 0;
	}
	else{
		return 1;
	}
}

void signal_handler (int sig) {
   // wait(NULL);
    printf("inside handler \n") ;
    FILE *fp;
    fp  = fopen ("data.log", "a");
    fprintf(fp, "\n Child process was terminated in signal handler \n");
    fclose(fp) ;
    return ;
}

// Executes the command given in an array of strings
void execute(char **commands){
	int ret;
	int i = 0 ;
    flag = 0 ;
    int stat;
    //******* & parameter *********
    while (commands[i] != NULL) {
        printf("commands= %s \n" , commands[i]) ;
        if (!strcmp(commands[i] , "&")){
            flag = 1 ;
            commands[i] = NULL ;
            break ;
        }
        i++ ;
    }
    //**********exit state***************
    if(strcmp(commands[0], "exit") == 0){

        printf("The Shell is exited now , Bye \n");
        exit(0);
    }
    if(strcmp(commands[0], "cd") == 0){
        printf("inside chdir function");
        chdir(commands[1]) ;
    }
    else {



	//forking
	pid_t pid = fork();
	if (pid == -1){		//failed
		printf("\nProccess forking failed!");
	}
	else if (pid == 0){	//child proccess
        printf("we are in child process \n") ;
		ret = execvp(commands[0],commands);            // execution
		printf("excution is done\n");
		if (ret< 0) {	//execusion failed
			printf("\nCommand execusion failed!");
		}
		exit(0);
	}
	else{			//parent section
        printf("we are in parent process \n") ;
		/* The parent should wait for the child proccess to terminate or not.
		 * This is determined by the user providing & as a last parameter or not.
		 * if given then the parent will NOT wait for the child to terminate and
		 * vice versa*/
		// TODO:handle the & parameter
		if (flag == 1) {    // & is found in the command >> child must operate in bachground >> parent shouldn't wait for the child
            flag = 0 ;
            printf("child is operating in background \n ") ;
           // signal(SIGCHLD , signal_handler ) ;
		}
        else if (flag == 0) {
            printf("waiting in else if = 0 \n") ;
           	//waits for the child to terminate
            printf("First Wait\n");

            waitpid(pid,&stat,0);
            if (WIFEXITED(stat)){
               printf("Exit status: %d\n", WEXITSTATUS(stat));
                printf("second Wait\n");

            }
            printf("(parent) child process is exited \n") ;
            //return ;
           	}

		return;
	}
	}
}
int main() {

	char *input_str , **parsed_tokens;
	int *pos = NULL;
	int i =0;
	signal(SIGCHLD , signal_handler ) ;
	while(1){
		if(get_command(input_str))
			continue;
		parsed_tokens = parse_command(input_str );


        printf("before execution in main \n") ;
		execute(parsed_tokens);
	}
}

