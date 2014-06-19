#include "includes/config_server.h"
//Dateicontainer
  struct FileInfo fileInfoRoot;    //Pointer auf erste FileInfo Struktur
  struct FileInfo fileInfoCurrent; //aktuelles Struct

//Daemon Variablen
  FILE *fp = NULL;
  char line[256];
  int pid;

//TCP-Verbindungsvariablen
  int server_socket, client_socket;
  struct sockaddr_in serverInfo, clientInfo;
  unsigned short serverPort, clientPort;
  unsigned int clientAddressSize;
  
int main(int argc, char* argv[]){
  //Benötigte Variablen
  //pid_t process_id = 0;
  //pid_t sid = 0;
  //char buffer[RCVBUFFSIZE];
  //int n, r;
  
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
  /*
  if((process_id = fork()) < 0){
    handleErrors(process_id, "fork failed\n");
    exit(1);
  }
  
  if(process_id > 0){ //Elternprozess beenden
    savePid(process_id);
    exit(0);
  }
  
  //Kindprozess übernimmt die Session.
  sid = setsid();
  handleErrors(sid, "Couldn't take session control");
  */
  //Reagiere auf Signale
  struct sigaction signal_handler;
		signal_handler.sa_handler = signalHandler; //Registriert signalHandler() als sa_handler
		sigemptyset(&signal_handler.sa_mask);
		sigaddset(&signal_handler.sa_mask, SIGTERM);
		signal_handler.sa_flags = 0;
		sigaction(SIGTERM, &signal_handler, NULL);
  
  //Oeffne TCP Socket
  server_socket = initServer();
    handleErrors(server_socket, "Socket opened\n");
    
  //unsigned int bytesReceived;
  
  //Starte Kindprozess für ankommende Clients
  while (TRUE){
			client_socket = accept(server_socket, (struct sockaddr *) &clientInfo, &clientAddressSize);
			handleErrors(client_socket, "client socket accepted");
      
			//Für jeden Client wird ein neuer Prozess gestartet
			pid_t pid;
			pid = fork();
			if (pid == -1){ 
				handleErrors(-1, "Couldn't start new Client-Process");
			}else if(pid == 0){				 
				handleErrors(1, "Client connected");
        printf("process started\n");
        
        //Reagiere auf ankommende Befehle, case switches (list, create, read, update, delete)
        //doProcessing(client_socket);
        
				exit(0);
			}
      close(client_socket);
  }
  
  
  
  return (0);
}

void doProcessing(int sock){
  int n;
  char buffer[256];

  bzero(buffer,256);

  n = read(sock,buffer,255);
  if (n < 0)
  {
      perror("ERROR reading from socket");
      exit(1);
  }
  printf("Here is the message: %s\n",buffer);
  n = write(sock,"I got your message\n",18);
  if (n < 0) 
  {
      perror("ERROR writing to socket");
      exit(1);
  }
}

/*
 * Funktionen für das starten, überprüfen und beenden des Daemons.
 */

//Speichert die Prozess-ID in run.pid
int savePid(int pid){
  if((fp = fopen(PIDFILE, "w+")) == NULL){
    handleErrors(-1, "couldn't open pid file\n");
    exit(1);
  }
  fprintf(fp, "%i",pid);
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
    handleErrors(-1, "couldn't pid file for stopping daemon!\n");
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
 * Funktionen für die TCP Verbindung
 */
 
//Starten des ServerSockets.
int initServer(){
  if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    handleErrors(server_socket, "Couldn't open Serversocket");
    return -1;
  }
    
	memset(&serverInfo, 0, sizeof(serverInfo));	
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);
	serverInfo.sin_port = htons(SRVPORT);
	
	int returnValue;
	returnValue = bind(server_socket, (struct sockaddr *) &serverInfo, sizeof(serverInfo));
	handleErrors(returnValue, "bind socket");
	returnValue = listen(server_socket, MAXCONNECTIONS);
	handleErrors(returnValue, "listen socket");
	clientAddressSize = sizeof(clientInfo);
	 
	return server_socket;
}

//Funktion zur Anzeige möglicher Fehlermeldungen, Fehler werden auf stderr ausgegeben.
void handleErrors(int number, const char *message){
	int errorNumber = errno;
	const char *errorstr = strerror(errorNumber);
	if (number < 0){
		if (message != NULL) {
			fprintf(stderr, " %s ", message);
			fprintf(stderr, "number : %d : %s\n", number, errorstr);
		}else{
			fprintf(stderr, "Undefined message received from server: handleErrors(%d): %s\n", number, errorstr);
		}
		exit(1); 
	}
	fflush(stderr);
}

//Signalbehandlung
void signalHandler(){
	handleErrors(-1, "Application will be stopped...\n");
  //Offene Sockets schliessen.
  close(server_socket);
	exit(0);
}
