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
#define DELIM " \t\r\n\a" //escape characters that separates tokensSS

int flag = 0 ; //set flag to indicate the & parameter

//---------function to parse command to separated tokens to obtain an array that contains the command and its arguments
char** parse_command(char *str ){
    int buf_size = MAX_BUF_SIZE , pos = 0;
	char **tokens_arr = malloc(buf_size * sizeof(char*));
	char *token;

	if(!tokens_arr){		//The malloc function did not complete
		printf("\nAllocation error!");
		exit(0);
	}
	token = strtok(str,DELIM);//spliting the command by DELM
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
		token = strtok(NULL,DELIM); //generate tokens according the DELIM entered by the user
	}
	tokens_arr[pos] = NULL; //make the last argument of the array NULL

	return tokens_arr;
}

//-------function to get command from the user using readline library and save it as a history
int get_command(char *str){
	char *buf;
	buf = readline("\n>>>");
	if (strlen(buf) != 0){
		add_history(buf);  //save the command in the history.By using the up arrow,you can get the last commands
		strcpy(str,buf);
		return 0;
	}
	else{
		return 1;
	}
}

//----------single Handler to write on the log file -----------------------------------
void signal_handler (int sig) {
    printf("inside handler \n") ;
    FILE *fp;
//---------*****IMP NOTE: should put fixed path to overcome any segmentation fault--------------
    fp  = fopen ("/home/nourhan/Nourhan/OS/Unix_shell/data.log", "a");
    fprintf(fp, "\n Child process was terminated in signal handler \n");
    fclose(fp) ;
    return ;
}

//---------------------Executes the command given in an array of tokens----------------------------
void execute(char **commands){
	int ret;
	int i = 0 ;
    flag = 0 ;
    int stat;

    //------handle & parameter--------------------------
    // by comparing the elements of token_array,Once it is founded, set the flag to be 1
    //and change the element by NULL
    while (commands[i] != NULL) {
        printf("commands= %s \n" , commands[i]) ;
        if (!strcmp(commands[i] , "&")){
            flag = 1 ; //set flag=1 indicating that this process will run in background
            commands[i] = NULL ;
            break ;
        }
        i++ ;
    }

    //-------------handle exit command-----------------------------
    //by comparing the elements of token_array,Once it is founded, print end message and close the terminal
    if(strcmp(commands[0], "exit") == 0){
        printf("The Shell is exited now , Bye \n");
        exit(0);
    }

    //--------------------handle cd command---------------------------
    //by comparing the elements of token_array,Once it is founded,change directory to what the user entered as argument
    if(strcmp(commands[0], "cd") == 0){
        printf("inside chdir function");
        chdir(commands[1]) ;
    }

//-----------------------------------child process creation and execution section-------------------------------
    else {
        pid_t pid = fork();//forking new child process

        if (pid == -1){		//failed to create child process
            printf("\nProccess forking failed!");
        }

    //----------------------child Process Section---------------------------
        else if (pid == 0){	//child proccess creation is succeeded
            printf("we are in child process \n") ;
            ret = execvp(commands[0],commands);      // execution the process
            printf("excution is done\n");

            if (ret< 0) {	//execution is failed
                printf("\nCommand execution failed!");
            }
            exit(0);
        }

    //------------------------------parent section------------------------------------
        /* The parent should wait for the child processes to terminate or not.
		 * This is determined by the user providing & as a last parameter or not.
		 * if given,then the parent will NOT wait for the child to terminate
		 */
        else{
            printf("we are in Parent Process \n") ;

    //-------------------------BackGround Case--------------------------*
            /* & is founded in the command,child must operate in background
            /*parent should NOT wait for the child to terminate.*/
            if (flag == 1) {
                flag = 0 ; //restore the flag to the default value which is zero
                printf("Child is operating in BackGround \n ") ;
            }

    //-------------------------ForeGround Case---------------------------*
            else if (flag == 0) {
                printf("Child is operating in ForeGround \n") ;
                printf("Waiting.....\n");
        //--------waiting the child process to terminate----------
            /*WIFEXITED macro will be TRUE if the child terminated normally.
            /*WEXITSTATUS macro is employed only if WIFEXITED returned true to
            /*check status of child process*/
            waitpid(pid,&stat,0);
            if (WIFEXITED(stat)){
                printf("Exit status: %d\n", WEXITSTATUS(stat));
                printf(" Wait is DONE \n");
            }
        //---------------------Execution is DONE successfully ---------------------------
            printf("(Parent) child process is exited \n") ;
           	}
        return;
        }
	}
}
//--------------------main function----------------------------------------------------------------
int main() {

	char *input_str , **parsed_tokens;
	int *pos = NULL;
	int i =0;
	//Using SIGCHILD signal which is sent to the parent process once the child process stops or terminates
	signal(SIGCHLD , signal_handler ) ;

	while(1){
		if(get_command(input_str))
			continue;
		parsed_tokens = parse_command(input_str );
		execute(parsed_tokens);
	}
}
