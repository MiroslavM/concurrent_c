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
    handleErrors(server_socket, "Server Socket not opened\n");
    
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
        //doProcessing(client_socket);
        //Reagiere auf ankommende Befehle, case switches (list, create, read, update, delete)
				int totalFileBytesReceived = 0;
        int t = 0;
				//Speichert die empfangenen optionen:list,create,read,update or delete
				char command[64]; 
            command[63] = '\0';
            memset(command, 0, sizeof(command));
				//Speichert den empfangenen Dateinamen
				char fileName[256];
            fileName[255] = '\0';
            memset(fileName, 0, sizeof(fileName));
				//Speichert die Grösse der Datei
				char fileSize[64];
            fileSize[63] = '\0';
            memset(fileSize, 0, sizeof(fileSize));
        
				//Datei zwischenspeichern, bevor sie in Shared Memory abgelegt wird.
				char *receiveBuffer = (char *) malloc(sizeof(char) * RCVBUFFSIZE);
				if (receiveBuffer == NULL){
					handleErrors(-1, "Memory allocation failed!");
          return -1;
				}
				 
				//Das empfangene Kommando des Clients überprüfen.
				int commandAction = 0;
				while(TRUE){
					char recBuffer[RCVBUFFSIZE];
					int recvMsgSize = recv(client_socket, recBuffer, RCVBUFFSIZE, MSG_DONTWAIT);
					 
					if (commandAction == 0){
							int cA;
							for (cA = 0; cA < RCVBUFFSIZE; cA++){
								if (recBuffer[cA] == '\n'){
                  cA++;
									break;
                }
							}
              
							//Empfangene Zeile splitten.
							char separator[]   = " \n";
							char *token;
							char *com = "";
							char *opt1 = "";
							char *opt2 = "";
              char *opt3 = "";
              
							token = strtok( recBuffer, separator );
							int counter = 0;
							while( token != NULL ){
								if (counter == 0){
									com = token;
								}else if(counter == 1){
									opt1 = token;
								}else if(counter == 2){
									opt2 = token;
								}else if(counter == 3){
                  opt3 = token;
                }
								token = strtok( NULL, separator );
								counter++;
							}
              
							//save received values
							snprintf(command, sizeof(com), "%s", com);
							if (!strncmp(command, "list", 4)){
                break;
							}else if(!strncmp(command, "create", 6)){
								snprintf(fileName, sizeof(fileName), "%s", opt1);
								snprintf(fileSize, sizeof(fileSize), "%s", opt2);
								break;
							}else if(!strncmp(command, "read", 4)){
								snprintf(fileName, sizeof(fileName), "%s", opt1);
                break;
							}else if(!strncmp(command, "update", 6)){
								snprintf(fileName, sizeof(fileName), "%s", opt1);
								snprintf(fileSize, sizeof(fileSize), "%s", opt3);
								break;
							}else if(!strncmp(command, "delete", 6)){
								snprintf(fileName, sizeof(fileName), "%s", opt1);
                break;
							}
							// Falls keine Daten angekommen sind...
							if (recvMsgSize == t){
                
							}else{
								totalFileBytesReceived = sizeof(recBuffer)-t-1;
								strncpy(receiveBuffer, recBuffer+t, sizeof(recBuffer)-t-1);
							}
							commandAction = 1;
					}else{
						totalFileBytesReceived += recvMsgSize;
						strncpy(receiveBuffer+strlen(receiveBuffer), recBuffer,sizeof(recBuffer));
						if (atoi(fileSize) == totalFileBytesReceived){
							break;
						}
					}
					memset(recBuffer, 0, sizeof(recBuffer));
				}
        
				//do job client requested
				if (!strncmp(command, "list", 4)){
					//listFiles(client_socket);
				}else if(!strncmp(command, "create", 6)){
          createFile(client_socket, receiveBuffer, fileName, fileSize);
				}else if(!strncmp(command, "read", 4)){
					//readFile(client_socket, fileName);
				}else if(!strncmp(command, "update", 6)){
					//updateFile(client_socket, recvBuffer, fileName, fileSize);
				}else if(!strncmp(command, "delete", 6)){
					//deleteFile(client_socket, fileName);
				}
				free(receiveBuffer);
				exit(0);
			}
  }
  
  
  
  return (0);
}

//Dateiliste an Client senden.
void listFiles(int socket){
  
}
//Dateien in Shared Memory abspeichern und an Dynamische Liste anketten.
void createFile(int socket, char *content, char *fileName, char *fileSize){
  printf("Creating Files\n");
}
//Datei an Client senden.
void readFile(int socket, char* fileName){
  
}
//Datei durch andere Datei ersetzen.
void updateFile(int socket, char *fileName, char *newFileName, char *fileSize, char *content){
  
}
//Datei löschen.
void deleteFile(int socket, char *fileName){
  
}

void doProcessing(int sock){
  int n,s;
  char buffer[256];

  bzero(buffer,256);

  n = read(sock,buffer,255);
  if (n < 0)
  {
      perror("ERROR reading from socket");
      exit(1);
  }
  printf("Here is the message: %s\n",buffer);
  s = send(sock,"I got your message\n",19, MSG_WAITALL);
  if (s < 0) 
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
  close(client_socket);
	exit(0);
}
