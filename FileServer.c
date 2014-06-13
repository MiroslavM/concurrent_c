#include "includes/config_server.h"

struct FileInfo fileInfoRoot;    //Pointer auf erste FileInfo Struktur
struct FileInfo fileInfoCurrent; //aktuelles Struct
FILE *fp = NULL;

int main(int argc, char* argv[]){
  //Benötigte Variablen
  //char fileName[256];
  
  //Starte Daemon
  pid_t process_id = 0;
  pid_t sid = 0;
  
  if((process_id = fork()) < 0){
    printf("fork failed\n");
    exit(1);
  }
  
  if(process_id > 0){
    printf("process_id of child process %d \n", process_id);
    savePid(process_id);
    exit(0);
  }
    
  //umask(0); set file creation mask to 0777
  
  //Kindprozess übernimmt die Session.
  if((sid = setsid()) < 0){
    printf("Couldn't take session control");
    exit(1);
  }
  
  while(1){
    sleep(100);
    
  }
  
  
  
  //Oeffne TCP Socket
  
  //Starte Kindprozess für ankommende Clients
  
  //Reagiere auf ankommende Befehle, case switches (list, create, read, update, delete)
  
  
  return (0);
}

//Speichert die Prozess-ID in run.pid
int savePid(int pid){
  if((fp = fopen(PIDFILE, "w+")) == NULL){
    printf("couldn't open file, following error occured: %s\n", strerror(errno));
    exit(1);
  }  
  fprintf(fp, "Logging test %i \n", pid);  
  fflush(fp);
  fclose(fp);
  exit(0);
}
