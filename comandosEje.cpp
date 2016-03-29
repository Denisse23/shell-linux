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
#include <sstream> 
#include <sys/utsname.h>
#include <signal.h>
#include <sys/stat.h> 
#include <fcntl.h>
using namespace std;

#define delim " \t\r\n\a"
#define tok_bufsize 255

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

inline bool isInteger(const std::string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

struct statInfo {
    string UID;
  	string PID;
    string VSZ;
    string RSS;
  	string TTY;
    string STAT;
    string START;
  	string TIME;
  	string CMD;
};

vector<string> split_linea(string line){
  vector<string> tokens;
  char * token;
  token = strtok ((char*)line.c_str(), delim);
  while (token != NULL){
    tokens.push_back(token);
    token = strtok (NULL, delim);
  }
	return tokens;
}

void ps( char const *argv[], int size){
	DIR *folder;
    struct dirent *entry;
    folder = opendir("/proc");
	

	vector<statInfo> psInfo;

    while(entry = readdir(folder)){
    	if (entry->d_type == DT_DIR) {
    		if (isInteger(entry->d_name)) {
    			DIR *proceso;
    			struct dirent *entry2;
    			string name = entry->d_name;
   				proceso = opendir((char *)("/proc/" + name).c_str());
   				psInfo.push_back(statInfo());

   				string cadena;
   				string archivoInfo ="";
    			ifstream fe((char *)("/proc/"+name+"/stat").c_str());
   				while(getline(fe,cadena))
    			{	
        			archivoInfo+= cadena;
       			
    			}
    			fe.close();
    			vector<string> statargs = split_linea(archivoInfo);

          string cadena2;
          string archivoInfo2 ="";
          ifstream fe2((char *)("/proc/"+name+"/status").c_str());
          while(getline(fe2,cadena2))
          { 
              archivoInfo2+= cadena2;
            
          }
          fe2.close();
          vector<string> statargs2 = split_linea(archivoInfo2);
          psInfo[psInfo.size()-1].UID = statargs2[8];
          psInfo[psInfo.size()-1].PID = entry->d_name;

          bool siVMZ = false;
          bool siRSS = false;
	       for(int i=0;i<statargs2.size();i++){
            if(statargs2[i]=="kBVmSize:"){
              psInfo[psInfo.size()-1].VSZ = statargs2[i+1];
              siVMZ = true;
            }
            if(statargs2[i]=="kBVmRSS:"){
              psInfo[psInfo.size()-1].RSS = statargs2[i+1];
              siRSS = true;
              break;
            }
         }
   			  if(!siVMZ)
            psInfo[psInfo.size()-1].VSZ = "0";
          if(!siRSS)
            psInfo[psInfo.size()-1].RSS = "0";
   				psInfo[psInfo.size()-1].TTY = statargs[6];
   				int time = atoi( statargs[13].c_str() )+ atoi(statargs[14].c_str()) + atoi(statargs[15].c_str())
   							+ atoi( statargs[16].c_str() );
   				stringstream stream;
   				stream << time;
          psInfo[psInfo.size()-1].STAT = statargs2[2]; 
          psInfo[psInfo.size()-1].START = statargs[21]; 
   				psInfo[psInfo.size()-1].TIME = stream.str(); 
   				psInfo[psInfo.size()-1].CMD = statargs[1];


                closedir(proceso);
            }

           
    	}

    }

     closedir(folder);
    
     if(size==1){

      printf(" %s\t%s\t%s\t%s\n","PID", "TTY","TIME", "CMD");
        for(int i=0;i<psInfo.size();i++){
           printf(" %s\t%s\t%s\t%s\n",(char *)psInfo[i].PID.c_str(),(char *)psInfo[i].TTY.c_str(),
               (char *)psInfo[i].TIME.c_str(), (char *)psInfo[i].CMD.c_str());
        
       }

     }else{
        printf(" %s\t%s\t%s\t\t%s\t%s\t%s\t%s\t%s\t%s\n","UID" ,"PID","VSZ","RSS", "TTY","STAT","START", "TIME", "CMD");
        for(int i=0;i<psInfo.size();i++){
        if(psInfo[i].VSZ.size()<=7){
            printf(" %s\t%s\t%s\t\t%s\t%s\t%s\t%s\t%s\t%s\n",(char *)psInfo[i].UID.c_str(), (char *)psInfo[i].PID.c_str(), (char *)psInfo[i].VSZ.c_str(), 
               (char *)psInfo[i].RSS.c_str(), (char *)psInfo[i].TTY.c_str(), (char *)psInfo[i].STAT.c_str(), (char *)psInfo[i].START.c_str(),
               (char *)psInfo[i].TIME.c_str(), (char *)psInfo[i].CMD.c_str());
        }else{
            printf(" %s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", (char *)psInfo[i].UID.c_str(),(char *)psInfo[i].PID.c_str(), (char *)psInfo[i].VSZ.c_str(), 
               (char *)psInfo[i].RSS.c_str(), (char *)psInfo[i].TTY.c_str(), (char *)psInfo[i].STAT.c_str(), (char *)psInfo[i].START.c_str(),
               (char *)psInfo[i].TIME.c_str(), (char *)psInfo[i].CMD.c_str());
        }
       }
      
     }

     

}

void uname ( char const *argv[], int size){
	struct utsname info;
  	int c =uname(&info);
  	if(size==2){
  		string param = argv[1];
  		switch (param[1]){
			case 's':
	  			cout<<info.sysname<<endl;
	  			break;
			case 'r':
	  			printf("%s\n",info.release);
	  			break;
			case 'm':
	  			printf("%s\n",info.machine);
	  			break;
			case 'a':
	  			printf("%s %s %s %s %s\n", info.sysname, info.nodename, info.release, info.version, info.machine);
	  			break;
		}
  	}else if(size==1){
  		printf("%s %s %s %s %s\n", info.sysname, info.nodename, info.release, info.version, info.machine);
  	}else{
  		printf("%s %s %s %s %s\n", info.sysname, info.nodename, info.release, info.version, info.machine);
  	}
  	
}

void kill_9 (char const *argv[], int size){
	if(size==1){
		 fprintf(stderr, "My_sh: sin argumentos para \"kill -9\"\n");
	}else{
		string PIDs  = argv[1];
		pid_t PID= atoi(PIDs.c_str());
		if(kill(PID,SIGKILL)!=0){
			string error = argv[1];
			string errorr = "MI_sh:kill -9:"+error;
			perror((char *)(errorr).c_str());
		}
	}
}

void chmod (char const *argv[], int size){
    if(size<3){
    	fprintf(stderr, "My_sh: sin argumentos para \"chmod\"\n");
    }else{
    	long i = strtol(argv[1], NULL, 8);
    	if (chmod (argv[2], i) < 0)
    	{
    		string error = argv[2];
    		string ee = argv[1];
			string errorr = "MI_sh:chmod:"+ee+":"+error;
       		perror((char *)(errorr).c_str());
   		}
	}
}

void ln_s (char const *argv[], int size){
    if(size<4){
    	fprintf(stderr, "My_sh: sin suficientes argumentos para \"ln -s\"\n");
    }else{
    	if (symlink(argv[2], argv[3]) !=0){
			string error = argv[2];
			string errorr = "MI_sh:ln -s:"+error;
       		perror((char *)(errorr).c_str());
		}
	}
}

void gedit(char const *argv[], int size){
  if(size==1){
    execl("/usr/bin/gedit", "/usr/bin/gedit", "Documento nuevo", NULL);
  }else{
    execl("/usr/bin/gedit", "/usr/bin/gedit", argv[1], NULL);
  }
}

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

void redireccionamiento(char const *argv[], int size){
  if(size<3){
      fprintf(stderr, "My_sh: sin suficientes argumentos\n");
    }else{
    int     fd[2];
    pid_t   childpid;
    pipe(fd);

    string linea = argv[1];
    linea.erase(linea.length()-1);
    vector <string> args = split_linea(linea);

    
    if(args.size()>2){
      fprintf(stderr, "My_sh: muchos argumentos\n");
    }else if(args.size()<=2 ){
      bool iscat = false;
      bool isps = false;
      char *argps[] = {(char *)linea.c_str(), NULL};
      if(linea!="ps"){
        if(args.size()==2 && args[0]=="cat"){
          iscat = true;
        }
      }else{
        isps=true;
        
      }
        if(iscat || isps){
        string file = argv[2];
        if((childpid = fork()) == -1){
            perror("fork");
        }else if(childpid == 0){
            fd[1] = dup(1);
            fd[0] = open(file.c_str(), O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
            dup2(fd[0], 1);
            
            if(iscat)
              execvp("comandosEje", getArgs(args));
            else
              execvp("comandosEje", argps);

        } else{
            close(fd[0]);
            fflush(stdout);
            dup2(fd[1], 1);
            close(fd[1]);

        }
            close(fd[0]);
            close(fd[1]);
        }else{
            printf("Comando no encontrado\n");
        }

    }
  }
}

void redireccionamientomas(char const *argv[], int size){
  if(size<3){
      fprintf(stderr, "My_sh: sin suficientes argumentos\n");
    }else{
    int     fd[2];
    pid_t   childpid;
    pipe(fd);

    string linea = argv[1];
    linea.erase(linea.length()-1);
    vector <string> args = split_linea(linea);

    
    if(args.size()>2){
      fprintf(stderr, "My_sh: muchos argumentos\n");
    }else if(args.size()<=2 ){
      bool iscat = false;
      bool isps = false;
      char *argps[] = {(char *)linea.c_str(), NULL};
      if(linea!="ps"){
        if(args.size()==2 && args[0]=="cat"){
          iscat = true;
        }
      }else{
        isps=true;
        
      }
        if(iscat || isps){
        string file = argv[2];
        if((childpid = fork()) == -1){
            perror("fork");
        }else if(childpid == 0){
            fd[1] = dup(1);
            fd[0] = open(file.c_str(), O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
            dup2(fd[0], 1);
            
            if(iscat)
              execvp("comandosEje", getArgs(args));
            else
              execvp("comandosEje", argps);

        } else{
            close(fd[0]);
            fflush(stdout);
            dup2(fd[1], 1);
            close(fd[1]);

        }
            close(fd[0]);
            close(fd[1]);
        }else{
            printf("Comando no encontrado\n");
        }

    }
  }
}

string comandos[] = {
  "mkdir",
  "cat",
  "rmdir",
  "rm",
  "rmdir -R",
  "ps",
  "uname",
  "kill -9",
  "chmod",
  "ln",
  "gedit",
  "redireccionamiento",
  "redireccionamientomas"
};

int size_comandos() {
 	return sizeof(comandos) / sizeof(char *);
}


void (*comandos_funciones[]) (char const *argv[], int size) = {
  	&mkdir,
  	&cat,
  	&rmdir,
  	&rm,
  	&rmdir_R,
  	&ps,
  	&uname,
  	&kill_9,
  	&chmod,
  	&ln_s,
    &gedit,
    &redireccionamiento,
    &redireccionamientomas
};


int main(int argc, char const *argv[]) {

	for (int i = 0; i < size_comandos(); i++) {
    	if (argv[0]== comandos[i]) {
      	   comandos_funciones[i](argv, argc);
    	}
  	}

	
	return 0;
}
