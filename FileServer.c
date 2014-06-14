#include "includes/config_server.h"

struct FileInfo fileInfoRoot;    //Pointer auf erste FileInfo Struktur
struct FileInfo fileInfoCurrent; //aktuelles Struct

FILE *fp = NULL;
char line[256];
int pid;

int main(int argc, char* argv[]){
  //Benötigte Variablen

  
  //Überprüfen ob der Prozess bereits läuft
  if(argc > 1){
    switch (strncmp(argv[1], "stop", 4)){
      case 0: if(stop() == 1){
      
      printf("Bye Bye \n");
      exit(0);
    }
      break;    
    }
  }
  
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
  fprintf(fp, "%i", pid);  
  fflush(fp);
  fclose(fp);
  exit(0);
}

int stop(){
  if((fp = fopen(PIDFILE, "r")) == NULL){
    printf("couldn't open file, following error occured: %s\n", strerror(errno));
    return -1;
  }
  while(fgets(line, 256, fp) != NULL)
  {
    sscanf (line, "%i", &pid);
    kill(pid, SIGTERM); //kill process
  }
   fclose(fp);
   unlink(PIDFILE); //delete run.pid
   return 1;
}
