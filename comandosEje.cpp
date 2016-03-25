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
using namespace std;

void mkdir(const char * direc){
	if (mkdir(direc, S_IRUSR | S_IWUSR | S_IXUSR) != 0){
		perror("Error");
	}
	
}

int main(int argc, char const *argv[]) {
	string comando = argv[0];

	if(comando == "mkdir"){
		mkdir(argv[1]);
	}
	return 0;
}
