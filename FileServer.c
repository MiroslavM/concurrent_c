#include "includes/config_server.h"
//Dateicontainer
  struct FileInfo *fileInfoBegin;    //Pointer auf erste FileInfo Struktur
  struct FileInfo *fileInfoEnd;      //aktuelles Struct
  
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
        //Reagiere auf ankommende Befehle, case switches (list, create, read, update, delete)
				int dataBytesReceived = 0;
        int cA = 0;
        
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
        
				//Datei zwischenspeichern, bevor sie in Shared Memory abgelegt wird. (Der Client muss nicht warten)
				char *receiveBuffer = (char *) malloc(sizeof(char) * RCVBUFFSIZE);
				if (receiveBuffer == NULL){
					handleErrors(-1, "Memory allocation failed!");
          return -1;
				}
        
				//Das empfangene Kommando des Clients überprüfen.
				int commandAction = 0;
				while(TRUE){
          char comBuffer[390];
					char recBuffer[RCVBUFFSIZE];
					int recvMsgSize = recv(client_socket, recBuffer, RCVBUFFSIZE, MSG_DONTWAIT);
            handleErrors(recvMsgSize, "No message received");
					if (commandAction == 0){
								for (cA = 0; cA < RCVBUFFSIZE; cA++){
                  comBuffer[cA] = recBuffer[cA];
                  if (recBuffer[cA] == '\n'){
                    cA++;
                    break;
                  }
                }
              printf("Command Buffer: %s", comBuffer);
							//Empfangene Zeile splitten.
							char separator[]   = " \n";
							char *token;
							char *com = "";
							char *opt1 = "";
							char *opt2 = "";
              char *opt3 = "";
							token = strtok(comBuffer,separator);
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
              
							snprintf(command, sizeof(com), "%s", com);
							if (!strncmp(command, "list", 4)){
                
                break;
							}else if(!strncmp(command, "create", 6)){
								snprintf(fileName, sizeof(fileName), "%s", opt1);
								snprintf(fileSize, sizeof(fileSize), "%s", opt2);
                unsigned int fileSizeInt = atoi(fileSize);
                //Zwischenspeicher, bevor die Datei in Shared Memory abgelegt wird. (Der Client muss nicht warten)
                char *receiveBuffer = (char *) malloc(sizeof(char) * fileSizeInt);
                if (receiveBuffer == NULL){
                  handleErrors(-1, "Memory allocation failed!");
                }
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
							if (recvMsgSize == cA){
                printf("No data\n");
							}else{ //Zeile nach Newline in Puffer speichern.
								dataBytesReceived += sizeof(recBuffer) - cA;
                strncpy(receiveBuffer, recBuffer, (sizeof(recBuffer)-cA-1));
							}
							commandAction = 1;
					}else{
						dataBytesReceived += recvMsgSize;
            strncpy(receiveBuffer+strlen(receiveBuffer)+1, recBuffer,sizeof(recBuffer));
            //Falls die Dateigrösse den empfangenen Bytes entspricht ist der Job erledigt.
						if(atoi(fileSize) == (dataBytesReceived - RCVBUFFSIZE) - 1){
							break;
						}
					}
					memset(recBuffer, 0, sizeof(recBuffer));
				}
				//do job client requested
				if (!strncmp(command, "list", 4)){          
					//listFiles(client_socket);
				}else if(!strncmp(command, "create", 6)){
          printf("create");
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
  //Solange FileInfo Structs existieren die Dateinamen und Grössen in buffer speichern
  while(fileInfoBegin->nextFile){
    
  }
}
//Dateien in Shared Memory abspeichern und an Dynamische Liste anketten.
void createFile(int socket, char *content, char *fileName, char *fileSize){
  unsigned int fileSizeInt = atoi(fileSize);
  //Shared Memory Segment und Semaphore anfordern, FileInfo verlinken.
	int fileExists = 0;
  //sem_t sem = NULL;
  int semId = 0;
  int shMemoryId = 0;
  char fN[5] = "File";
  
  //Hinzufügen von Dateien soll exclusiv erfolgen. http://openbook.galileocomputing.de/unix_guru/node394.html
	void append(int semId, unsigned int fileSizeInt, char fN, int shMemoryId);
  
  printf("After append: %i%i%i%s%i\n", semId, shMemoryId, fileSizeInt, fN, fileInfoBegin->fileSize);
  
	// Antwort an Client 
	if (fileExists == 1){
		char result[14];
		result[13] = '\0';
		snprintf(result, 13, "%s", "FILE EXISTS\n");
		send(socket, result, 13, 0);
	}else{
		char result[15];
		result[14] = '\0';
		snprintf(result, 14, "%s", "FILE CREATED\n");
		send(socket, result, 14, 0);
	}
  
}
//Datei an Client senden.
void readFile(int socket, char* fileName){
  //Shared Memory Segment direkt an den Client übertragen, falls Datei vorhanden. Benötigt keine weiteren Absicherungen (semaphore)
  
}
//Datei durch andere Datei ersetzen.
void updateFile(int socket, char *fileName, char *newFileName, char *fileSize, char *content){
  /*Zuerst wird überprüft, ob Datei vorhanden dann wird das bisherige Segment gelöscht und ein neues angelegt,
   um es anschliessend mit dem ursprünglichen FileInfo zu verbinden.
   * */
}


/*
 * Funktionen, welche die Rückgabewerte an die Clients senden.
 * */
void sendListResult(int socket){
  
}
void sendCreateResult(int socket){
  
}
void sendReadResult(int socket){
  
}
void sendUpdateresult(int socket){
  
}
void sendDeleteFileResult(int socket){
  
}

//Initialisierung der FileInfo Liste
void start(void) {
   fileInfoBegin = fileInfoEnd = NULL;
}

//Anhängen von weiteren FileInfo Structs
void append(int semId, unsigned int fSize, char fName, int shMemoryId) {
  printf("appending1");
   /* Zeiger zum Zugriff auf die einzelnen Elemente
    * der Struktur */
   struct FileInfo *zeiger, *zeiger1;

   /* Wurde schon Speicher für den ende-Zeiger bereitgestellt? */
   if(fileInfoEnd == NULL) {
      if((fileInfoEnd=malloc(sizeof(struct FileInfo))) == NULL) {
         printf("Konnte keinen Speicherplatz für ende "
                "reservieren\n");
         return;
      }
   }

   /* Wir fragen ab, ob es schon ein Element in der Liste gibt.
    * Wir suchen das Element, auf das unser Zeiger *anfang
    * zeigt. Falls *anfang immer noch auf NULL zeigt, bekommt
    * *anfang die Adresse unseres 1. Elements und ist somit der
    * Kopf (Anfang) unserer Liste. */
   if(fileInfoBegin == NULL) {
      /* Wir reservieren Speicherplatz für unsere
       * Struktur für das erste Element der Liste. */
      if((fileInfoBegin = malloc(sizeof(struct FileInfo))) == NULL) {
         handleErrors(-1 ,"No more space in memory");
         return;
      }
      
      //Die Informationen zur DateiStruktur werden hier gesetzt.
      //fileInfoBegin->semaphore = sem;
      fileInfoBegin->semaphoreId = semId;
      fileInfoBegin->fileSize = fSize;
      fileInfoBegin->fileName = fName;
      fileInfoBegin->sharedMemoryId = shMemoryId;
      
      //Zeiger auf die Korrekten Structs setzen.
      fileInfoBegin = fileInfoEnd;
      fileInfoEnd->prevFile=NULL;
   }else {
      zeiger = fileInfoBegin;    /* Wir zeigen auf das 1. Element. */
      while(zeiger->nextFile != NULL)
         zeiger = zeiger->nextFile;
      /* Wir reservieren einen Speicherplatz für das letzte
       * Element der Liste und hängen es an. */
      if((zeiger->nextFile = malloc(sizeof(struct FileInfo))) == NULL) {
         handleErrors(-1, "No more space in memory");
         return;
      }
      
      zeiger1 = zeiger; //zeiger1 ist die aktuell letzte Struktur

      zeiger=zeiger->nextFile; /* zeiger auf neuen Speicherplatz */
      //strcpy(zeiger->name,strtok(n, "\n"));
      zeiger->fileSize = fSize;
      zeiger->nextFile = NULL;
      
      //Zeiger auf die Korrekten Structs setzen
      fileInfoEnd = zeiger;
      zeiger->prevFile = zeiger1;
      zeiger1->nextFile = zeiger;
    }
}

int getShmId(){
	/*shmId = (int *) shmat(shmIdPointer, NULL, 0);
	int varShmId = *shmId;
	res = shmdt(shmId);
	handleErrors(res, "Shared Memory Segment not attached!");
	return storshmid;*/
  return 0;
}
//Funktion um ein Struct aus der Liste zu entfernen.
void detach(char *fileName){
  //http://openbook.galileocomputing.de/c_von_a_bis_z/021_c_dyn_datenstrukturen_002.htm#mj0230cca6aa7fb87181f7f69e396d6fd8
  
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
