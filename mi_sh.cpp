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

#define tok_bufsize 255
#define directory_buf_size 255
#define delim " \t\r\n\a"

///////////////////// Funciones comandos/////////////////////////////////////////////////////////

void C_cd(vector<string>);
void C_exit(vector<string>);
void C_mkdir(vector<string>);
void C_cat(vector<string>);
void C_rmdir(vector<string>);
void C_rm(vector<string>);
void C_rmdir_R(vector<string>);


//////////////////////////////////////////// LINEA////////////////////////////////////////////////////////

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

vector<string> revisarlinea( char * line,vector<string> args){
	bool charfoundb = false;
 	const char *charfound;
	for(int i=0;i<args.size();i++){
    if(args[i]=="|" || args[i]==">" || args[i]=="<"){
      charfoundb=true;
      charfound = args[i].c_str();
    }
  }
  vector<string> tokens;
  if(charfoundb){
    
    char * token;
    token = strtok (line, charfound);
    while (token != NULL){
      tokens.push_back(token);
      token = strtok (NULL, charfound);
    }
  }
  return tokens;

}

/////////////////////////////////////// Arreglo de argumentos para hacer llamada/////////////////////////

char ** getArgs(vector<string> args){
	char **tokens =(char**) malloc(tok_bufsize * sizeof(char*));
	int position = 0;
	for(int i=0; i<args.size();i++){
		tokens[position] = (char*)args[i].c_str();
		position++;
	}
	tokens[position] = NULL;
	return tokens;
}

/////////////////////////////// LLamadas al sistema con EXECVP -- al ejecutable: comandosEje//////////////////////////

void call_system(vector<string> args){
    pid_t pid, wpid;
    int status;
    pid = fork();

	if (pid==0) {
      execvp("comandosEje", getArgs(args));
      	//Child process
    } else if (pid < 0) {
      // Error forking
      perror("fork");
    } else {
      // Parent process
      do {
          wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}


////////////////////////////////////////////////lista de comandos///////////////////////////
string comandos[] = {
  "cd",
  "exit",
  "mkdir",
  "cat",
  "rmdir",
  "rm",
  "rmdir -R"
};

int size_comandos() {
 	return sizeof(comandos) / sizeof(char *);
}

///////////////////////////// lista de funciones de comandos ///////////////////////////////////

void (*comandos_funciones[]) (vector<string>) = {
 	&C_cd,
 	&C_exit,
  	&C_mkdir,
  	&C_cat,
  	&C_rmdir,
  	&C_rm,
  	&C_rmdir_R
};

/////////////////////////////////////////// Ejecutar comando ///////////////////////////////////////

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

////////////////////////////////////// Redireccionamiento o tuberias ////////////////////////////////

void ejecutarTubOredic(vector<string> args)
{
   printf("NO hay nada por aqui todavia\n");

} 

////////////////////////////////////////////////////////// MAIN /////////////////////////////////////////////////

int main(int argc, char **argv){

	char directory [directory_buf_size];
	getcwd(directory, directory_buf_size);
	string directory_string = "PATH=";
  	string direc = directory;
  	directory_string+=direc;
	putenv((char *)directory_string.c_str());


	char *line;
 	vector<string> args;
  	vector<string> argsTubOredic;

  	do {
  		getcwd(directory, directory_buf_size);
  		string direct = directory;
  		string path = "MI_sh>:~"+direct+"$ ";
    	printf("%s",(char *)path.c_str());
    	line =read_line();
      	args = split_linea(line);
      	argsTubOredic = revisarlinea(line, args);
      	if(argsTubOredic.size()==0){
        	ejecutar(args);
      	}else{
         	ejecutarTubOredic(argsTubOredic);
     	}
    	
    	

  	} while (true);

  	return 0;
}

////////////////////////////////////////////////// COMANDOS ////////////////////////////////////////////////

void C_cd(vector<string> args){
	if (args.size() == 1) {
  		fprintf(stderr, "My_sh: sin argumentos para \"cd\"\n");
  	} else {
    	if (chdir(args[1].c_str()) != 0) {
      		perror((char *)("MI_sh:cd:"+args[1]).c_str());
    	}	
  	}
 	
}


void C_exit(vector<string> args)
{
	exit(0);
}

void C_mkdir(vector<string> args){
	call_system( args);
 }

 void C_cat(vector<string> args){
	call_system( args);
 }

 void C_rmdir(vector<string> args){
 	if(args.size()==3){
 		args[0] = args[0]+" "+args[1];
		args.erase(args.begin() + 1);
		ejecutar(args);
 	}else{
 		call_system( args);
 	}
	
 }

 void C_rm(vector<string> args){
	call_system( args);
 }
 void C_rmdir_R(vector<string> args){
	call_system( args);
 }
