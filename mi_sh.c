#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>

#define tok_bufsize 64
#define delim " \t\r\n\a"

char *read_line()
{
	char *line = NULL;
 	ssize_t bufsize = 0;
 	getline(&line, &bufsize, stdin);
  	return line;
}

char **split_linea(char *line){
	int bufsize = tok_bufsize, position = 0;
 	char **tokens = malloc(bufsize * sizeof(char*));
 	char *token;
	token = strtok(line, delim);
  	while (token != NULL) {
    	tokens[position] = token;
    	position++;

    	if (position >= bufsize) {
      		bufsize += tok_bufsize;
      		tokens = realloc(tokens, bufsize * sizeof(char*));
      
   		}
		token = strtok(NULL, delim);
  	}
  	tokens[position] = NULL;
  	return tokens;
}


/*
  	//comandos ---------------------------------------------------------------------------
*/
char *comandos[] = {
  "cd",
  "exit"
};

int C_cd(char **args){
	if (args[1] == NULL) {
  		fprintf(stderr, "My_sh: sin argumentos para \"cd\"\n");
  	} else {
    	if (chdir(args[1]) != 0) {
      		perror("My_sh");
    	}	
  	}
 	return 1;
}



int C_exit(char **args)
{
	return 0;
}

int (*comandos_funciones[]) (char **) = {
 	&C_cd,
 	&C_exit
};

int size_comandos() {
 	return sizeof(comandos) / sizeof(char *);
}



int comandos_Execvp(char **args)
{
 	pid_t pid, wpid;
  	int status;

 	pid = fork();
  	if (pid == 0) {
    	// Child process
    	if (execvp(args[0], args) == -1) {
      	perror("MI_sh>");
    	}
    	exit(EXIT_FAILURE);
  	} else if (pid < 0) {
    	// Error forking
    	perror("MI_sh>");
  	} else {
    	// Parent process
    	do {
      		wpid = waitpid(pid, &status, WUNTRACED);
   		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
  	}

  	return 1;
}



int ejecutar(char **args)
{
 	int i;

  	if (args[0] == NULL) {
    	// No se ingresaron comandos
     	return 1;
  	}

  	for (i = 0; i < size_comandos(); i++) {
    	if (strcmp(args[0], comandos[i]) == 0) {
      		return (*comandos_funciones[i])(args);
    	}
  	}

  	return comandos_Execvp(args);
}


int main(int argc, char **argv){
  
	char *line;
 	char **args;
  	int status;

  	do {
    	printf("MI_sh>");
    	line = read_line();
    	args = split_linea(line);
    	status = ejecutar(args);

    	free(line);
    	free(args);
  	} while (status);

  	return 0;
}

