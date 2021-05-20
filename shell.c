#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

//these libraries are used to read line from stdin and also having a history of commands
#include<readline/readline.h>
#include<readline/history.h>

#define MAX_BUF_SIZE 64 //maximum number of characters for any buffer
#define DELIM " \t\r\n\a" //escape characters that seperates tokens

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
	char *input_str , **parsed_tokens;
	int *pos = NULL;
	int i =0;
	while(1){
		if(get_command(input_str))
			continue;
		parsed_tokens = parse_command(input_str );
                 /*while(parsed_tokens[i]!= NULL ){
                         printf("%s\n",parsed_tokens[i]);
                         i++;
                 }*/

		execute(parsed_tokens);
		//free(input_str);
		//free(parsed_tokens);
		//pos = NULL;

	}

}
