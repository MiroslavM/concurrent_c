#include "../includes/config_client.h"
  
  //TCP-Verbindungsvariablen
  int server_socket, client_socket;
  struct sockaddr_in serverInfo, clientInfo;
  unsigned short serverPort, clientPort;
  unsigned int serverAddressSize;
  
  //Dateinamen
  char *localFileName, *remoteFileName;
  
  char receiveBuffer[RCVBUFFSIZE];
  int n, w;
  
int main(int argc, char* argv[]){
    
	//Socket für Kommunikation mit Server öffnen.
	int socket = initClientConnection();
  handleErrors(socket, "Client Socket not opened\n");
  
  //Überprüfen ob der Dateiname länger als 256 Zeichen ist
  if(argc > 2 && !checkFilename(argv[2])){
    localFileName = argv[2];
  }
  
  int command = clientCommandParser(argc, argv);
  switch (command){
    case 0:
    break;
    case 1:
      fileList(socket);
    break;
    case 2:
      fileCreate(socket, localFileName);
    break;
    case 3:
      fileRead(socket, localFileName);
    break;
    case 4:
      fileUpdate(socket, localFileName, remoteFileName);
    break;
    case 5:
      fileDelete(socket, localFileName);
    break;
  }
  //fileList(socket);
  //getListResult(socket);
	
	exit(0);
  
}

int clientCommandParser(int argc, char **argv){
	 if(argc == 1){
    howToUse(argv);
    return 0;
   }else if(argc > 1){
      if (!strcmp(argv[1], "LIST") || !strcmp(argv[1], "list")){
          return 1;
          printf("List command set");
          
          
      }else if(!strcmp(argv[1], "CREATE") || !strcmp(argv[1], "create")){
        
          return 2;
          
      }else if(!strcmp(argv[1], "READ") || !strcmp(argv[1], "read")){
          printf("R command set");
          return 3;
          
          
      }else if(!strcmp(argv[1], "UPDATE") || !strcmp(argv[1], "update")){
          printf("U command set");
          return 4;
          
          
      }else if(!strcmp(argv[1], "DELETE") || !strcmp(argv[1], "delete")){
          printf("D command set");
          return 5;
          
          
      }else{
        howToUse(argv);
        return 0;
      }
   }
   return 0;
}

//Verbindung zu Server öffnen
int initClientConnection(){
  char *serverIP = SRVIP;
	serverPort = SRVPORT;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	handleErrors(client_socket, "Client socket opened!");
	
	memset(&serverInfo, 0, sizeof(serverInfo));
	serverInfo.sin_family	= AF_INET;
	serverInfo.sin_addr.s_addr = inet_addr(serverIP);
	serverInfo.sin_port = htons(SRVPORT);
	
	int res;
	res = connect(client_socket, (struct sockaddr *) &serverInfo, sizeof(serverInfo));
	handleErrors(res, "Client not connected");
	serverAddressSize = sizeof(clientInfo);
	return client_socket;
  
}

void fileList(int socket){
	char *action = malloc(sizeof(char) * 6);
	action[5] = '\0';
        snprintf(action, 5, "%s", "list\n");
        send(socket, action, 5, 0);
	free(action);
}

//Sendet eine Datei an den Socket
void fileCreate(int socket, char *filename){
  //int checkFilename(char *filename);
  FILE *filePointer;
	filePointer = fopen(filename, "r");
	if (filePointer == NULL){
		handleErrors(-1, "File couldn't been opened!");
	}
  //Dateigrösse ermitteln
  int fd = fileno(filePointer);
  struct stat buf;
  fstat(fd, &buf);
  unsigned int size = buf.st_size;
  
  int msgsize = size + 7 + 5;
  char *command = malloc(sizeof(char) * size);
  snprintf(command, msgsize, "%s %s %s %u %s", "create", filename, " ", size, "\n");
	send(socket, command, msgsize-1, 0);
  
	free(command);
  fclose(filePointer);
  
  /*
	//Dateigrösse überprüfen.
	
	 
	//set pointer to the beginning of the file
        fseek(fp, 0L, SEEK_SET);
	
	unsigned int totalbytes;
        totalbytes = 0;
	
	char file[BUFFERSIZE];
        size_t bytes = 0;

	// do not send all at once
	while ( (bytes = fread(file, sizeof(char), BUFFERSIZE, fp)) > 0){
		int offset = 0;
        	int sent;
		while ((sent = send(Socket, file + offset, bytes, 0)) > 0) {
			totalbytes += sent;
                        offset += sent;
                        bytes -= sent;
		}
		
	}
        fclose(fp);
        */
}

void fileRead(int Socket, char *filename){

}

void fileUpdate(int Socket, char *localFilename, char *remoteFilename){
  
}

void fileDelete(int Socket, char *remoteFilename){
  
}

int checkFilename(char *filename){
  if(sizeof(filename)>256){
    printf("Filename length is limited to 256 characters.\n\r");
    return (-1);
  }
  return 0;
}

void getListResult(int socket){
	while(TRUE){
    printf("empfang: \n");
		int received = recv(socket, receiveBuffer, RCVBUFFSIZE-1, MSG_DONTWAIT); //Empfängt die Daten im Non-Blocking Modus
		if (received < 0 && errno == EAGAIN) {
			//Keine Daten oder Socket nicht lesbar...
      handleErrors(received, "Socket not readable\n");
			continue;
		}
    
		}
    //close(socket);
}

void howToUse(char **argv){
  printf("How to use:\n");
  printf("\tlist files   :  # %s LIST\n", argv[0]);
  printf("\tcreate file  :  # %s CREATE\t <local-filename>\n", argv[0]);
  printf("\tread file    :  # %s READ\t <remote-filename>\n", argv[0]);
  printf("\tupdate file  :  # %s UPDATE\t <remote-filename> <local-filename>\n", argv[0]);
  printf("\tdelete file  :  # %s DELETE\t <remote-filename>\n\n", argv[0]);
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
