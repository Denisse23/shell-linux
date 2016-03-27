#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <sys/wait.h>
#include <string>
#include <cstdlib>
#include <dirent.h> 
#include <fstream>
using namespace std;

void mkdir(char const *argv[], int size){
	if (size == 1) {
      fprintf(stderr, "My_sh: sin argumentos para \"mkdir\"\n");

	}else if (mkdir(argv[1], S_IRUSR | S_IWUSR | S_IXUSR) != 0){
		string error = argv[1];
		string errorr = "MI_sh:mkdir:"+error;
		perror((char *)(errorr).c_str());
	}
	
}

void cat(char const *argv[], int size){
	string cadena;
    ifstream fe(argv[1]);

    if(size==1){
    	fprintf(stderr, "My_sh: sin argumentos para \"cat\"\n");
    }else{
    	if(fe.fail()){
    		string error = argv[1];
			string errorr = "MI_sh:cat:"+error;
    	  	perror( (char *) errorr.c_str());
    	}else{
   			while(getline(fe,cadena))
    		{	
        		cout<<cadena<<endl;
       			
    		}
    		fe.close();
    	}
	}

}

void rmdir(char const *argv[], int size){
	if (size == 1) {
      fprintf(stderr, "My_sh: sin argumentos para \"rmdir\"\n");

	}else if (rmdir(argv[1])){
		string error = argv[1];
		string errorr = "MI_sh:rmdir:"+error;
		perror((char *)(errorr).c_str());
	}
	
}
void rm(char const *argv[], int size){
	if (size == 1) {
      fprintf(stderr, "My_sh: sin argumentos para \"rm\"\n");

	}else if (unlink(argv[1])){
		string error = argv[1];
		string errorr = "MI_sh:rm:"+error;
		perror((char *)(errorr).c_str());
	}
	
}


void eliminarDirectorio(char *directorio){
	DIR *folder;
    struct dirent *entry;
    string nombre;
    folder = opendir(directorio);

    while(entry = readdir(folder))
    	if (entry->d_type == DT_DIR) {
    		if (strcmp(entry->d_name, ".") != 0 &&
            strcmp(entry->d_name, "..") != 0) {
    			string dir = directorio;
    			string dirdos = entry->d_name;
                eliminarDirectorio((char*)(dir+"/"+dirdos).c_str());
                
            }
    	}else{
    		string name = directorio;
    		string namedos = entry->d_name;
    		unlink((char *)(name+"/"+namedos).c_str());
    		
    	}

    closedir(folder);
    rmdir(directorio);

}

void rmdir_R(char const *argv[], int size){
	if (size == 1) {
      fprintf(stderr, "My_sh: sin argumentos para \"rmdir -R\"\n");

	}else {
		DIR *folder;
	  	if (folder = opendir(argv[1])){
	  		closedir(folder);
	  		string name = argv[1];
			eliminarDirectorio((char *)name.c_str());
	  	}else{
			string error = argv[1];
			string errorr = "MI_sh:rmdir -R:"+error;
			perror((char *)(errorr).c_str());
		}
	}
}

string comandos[] = {
  "mkdir",
  "cat",
  "rmdir",
  "rm",
  "rmdir -R"
};

int size_comandos() {
 	return sizeof(comandos) / sizeof(char *);
}


void (*comandos_funciones[]) (char const *argv[], int size) = {
  	&mkdir,
  	&cat,
  	&rmdir,
  	&rm,
  	&rmdir_R
};


int main(int argc, char const *argv[]) {

	for (int i = 0; i < size_comandos(); i++) {
    	if (argv[0]== comandos[i]) {
      	   comandos_funciones[i](argv, argc);
    	}
  	}

	
	return 0;
}
