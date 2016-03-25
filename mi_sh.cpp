#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <sys/wait.h>
#include <string>
#include <cstdlib>
using namespace std;

#define tok_bufsize 200
#define directory_buf_size 200
#define delim " \t\r\n\a"

char directory[directory_buf_size];
string directory_string;

char *read_line(){
  static char line[tok_bufsize];
  cin.getline(line, tok_bufsize);
  return line;
}

vector<string> split_linea(char *line){
  vector<string> tokens;
  char * token;
  token = strtok (line, delim);
  while (token != NULL){
    tokens.push_back(token);
    token = strtok (NULL, delim);
  }
	return tokens;
}

vector<string> revisarlinea(vector<string> args){
  string line = "";
  bool charfoundb = false;
  const char *charfound;
  for(int i=0;i<args.size();i++){
    line+= args[i];
    if(args[i]=="|" || args[i]==">" || args[i]=="<"){
      charfoundb=true;
      charfound = args[i].c_str();
    }
  }
  vector<string> tokens;
  if(charfoundb){
    
    char * token;
    token = strtok (const_cast<char*>(line.c_str()), charfound);
    while (token != NULL){
      tokens.push_back(token);
      token = strtok (NULL, charfound);
    }
  }
  return tokens;

}


void call_system(vector<string> args){
    pid_t pid, wpid;
    int status;
    pid = fork();

    if (pid==0) {
      char *argsinterno[] =  {(char *)args[0].c_str(),(char *)args[1].c_str(), (char *)0};
      execvp((directory_string + "/comandosEje").c_str(),argsinterno);

    } else if (pid < 0) {
      // Error forking
      perror("Error");
    } else {
      // Parent process
      do {
          wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}


/*
  	//comandos ---------------------------------------------------------------------------
  	//LOS comandos que no hacen execvp son: cd y exit
*/
string comandos[] = {
  "cd",
  "exit",
  "mkdir"
};

void C_cd(vector<string> args){
	if (args.size() == 1) {
  		fprintf(stderr, "My_sh: sin argumentos para \"cd\"\n");
  	} else {
    	if (chdir(args[1].c_str()) != 0) {
      		perror("Error");
    	}	
  	}
 	
}


void C_exit(vector<string> args)
{
	 exit(0);
}

void C_mkdir(vector<string> args){
  if (args.size() == 1) {
      fprintf(stderr, "My_sh: sin argumentos para \"mkdir\"\n");
    } else {
      call_system( args);
    }
}


void (*comandos_funciones[]) (vector<string>) = {
 	&C_cd,
 	&C_exit,
  &C_mkdir
};

int size_comandos() {
 	return sizeof(comandos) / sizeof(char *);
}


void ejecutar(vector<string> args)
{
 	int i;

    bool comandoencontrado = false;
    
  	for (i = 0; i < size_comandos(); i++) {
    	if (args[0]== comandos[i]) {
           comandoencontrado=true;
      	   comandos_funciones[i](args);
    	}
  	}
    if(!comandoencontrado){
        printf("Comando no encontrado\n");
    }
  	
} 

void ejecutarTubOredic(vector<string> args)
{
   printf("NO hay nada por aqui todavia\n");

} 




int main(int argc, char **argv){
  getcwd(directory, directory_buf_size);
  directory_string = directory;
	char *line;
 	vector<string> args;
  vector<string> argsTubOredic;

  	do {
    	printf("MI_sh>");
    	line =read_line();
      args = split_linea(line);
      argsTubOredic = revisarlinea(args);
      if(argsTubOredic.size()==0){
          ejecutar(args);
      }else{
         ejecutarTubOredic(argsTubOredic);
      }
    	
    	

  	} while (true);

  	return 0;
}

