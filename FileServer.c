#include "includes/config_server.h"
//Dateicontainer
  struct FileInfo fileInfoRoot;    //Pointer auf erste FileInfo Struktur
  struct FileInfo fileInfoCurrent; //aktuelles Struct

//Daemon Variablen
  FILE *fp = NULL;
  char line[256];
  int pid;

//TCP-Verbindungsvariablen
  int server_socket; /* Socket descriptor for server */
  int client_socket; /* Socket descriptor for client */
//  struct sockaddr_in server_address = "127.0.0.1"; /* Local address */
  struct sockaddr_in client_address; /* Client address */
  unsigned short server_port = SRVPORT; /* Server port */
  unsigned int client_address_len; /* Length of client address data structure */

int main(int argc, char* argv[]){
  //Benötigte Variablen
  pid_t process_id = 0;
  pid_t sid = 0;
  
  //Falls der Benutzer stop eingibt.
  if(argc > 1){
    switch (strncmp(argv[1], "stop", 4)){
      case 0: if(stop() == 1){
      
      printf("Bye Bye \n");
      exit(0);
    }
      break;    
    }
  }

  //Überprüfen ob der Prozess bereits läuft, sonst wird der Daemon gestartet
  if(checkProcessState() == TRUE){
    printf("Server Process is allready running\n");
    exit(1);
  }
  
  if((process_id = fork()) < 0){
    printf("fork failed\n");
    exit(1);
  }
  
  if(process_id > 0){ //Elternprozess beenden
    savePid(process_id);
    exit(0);
  }
  
  //Kindprozess übernimmt die Session.
  if((sid = setsid()) < 0){
    printf("Couldn't take session control");
    exit(1);
  }
  while(TRUE){
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

int checkProcessState(){
  if((fp = fopen(PIDFILE, "r")) == NULL){ //kann die Datei nicht öffnen
    return FALSE;
  }
  else{
    while(fgets(line, 256, fp) != NULL){
      sscanf (line, "%i", &pid);
    }
    if((kill(pid, 0)) == 0){ //Prozess ist gestartet
      return TRUE;
    }
    return FALSE;
  }
  return TRUE;
}

//Stoppt den Prozess und entfernt die run.pid Datei.
int stop(){
  if((fp = fopen(PIDFILE, "r")) == NULL){
    printf("couldn't open file, following error occured: %s\n", strerror(errno));
    return -1;
  }
  while(fgets(line, 256, fp) != NULL){
    sscanf (line, "%i", &pid);
  }
   kill(pid, SIGTERM); //kill process
   fclose(fp);
   unlink(PIDFILE); //delete run.pid
   return 1;
}
/*
int initSocket(){
	serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(SRVADDR);
	serverAddr.sin_port = htons(SRVPORT);
	
	int rc;
	rc = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	rc = listen(serverSocket, MAXCONNECTIONS);
	clientAddrLen = sizeof(clientAddr);
	 
	return serverSocket;

}
*/
