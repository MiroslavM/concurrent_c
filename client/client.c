#include "../includes/config_client.h"
  
  //TCP-Verbindungsvariablen
  int server_socket, client_socket;
  struct sockaddr_in serverInfo, clientInfo;
  unsigned short serverPort, clientPort;
  unsigned int serverAddressSize;
  
int main(int argc, char* argv[]){
  clientCommandParser(argc, argv);

  
	//Socket für Kommunikation mit Server öffnen.
	int socket = initClientConnection();
  fileList(socket);
	
	exit(0);
  
}

void howToUse(char **argv){
	printf("Usage:\n");
	printf("\tlist files   :  # %s list\n", argv[0]);
	printf("\tcreate file  :  # %s create <local-filename>\n", argv[0]);
	printf("\tread file    :  # %s read <remote-filename>\n", argv[0]);
	printf("\tupdate file  :  # %s update <remote-filename> <local-filename>\n", argv[0]);
	printf("\tdelete file  :  # %s delete <remote-filename>\n\n", argv[0]);
	exit(0);
}

void clientCommandParser(int argc, char **argv){
	 
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
	handleErrors(res, "Client ist verbunden");
	serverAddressSize = sizeof(clientInfo);
	return client_socket;
  
}

void fileList(int socket){
	char *action = malloc(sizeof(char) * 6);
	action[5] = '\0';
        snprintf(action, 5, "%s", "list\n");
        send(Socket, action, 5, 0);
	free(action);
}

void fileCreate(int Socket, char *filename){
  
}

void fileRead(int Socket, char *filename){

}

void fileUpdate(int Socket, char *localFilename, char *remoteFilename){
  
}

void fileDelete(int Socket, char *remoteFilename){
  
}

void getListResult(int Socket){
	int filesOnServer = 0;
	int receivedFiles = 0;
	char rBuffer[BUFFERSIZE];
	while(TRUE){
		int size = recv(Socket, rBuffer, BUFFERSIZE-1, MSG_DONTWAIT); 
		if (size < 0 && errno == EAGAIN) {
			/* no data for now, call back when the socket is readable */
			continue;
		}
		if (parseAction == 0){
			char *rBufferSave = strdup(rBuffer);
			char *token = strtok( rBuffer, " \n" );
			printf("%s ", token);
			token = strtok( NULL, " \n" );
			numberoffiles = atoi(token);
			printf("%d\n", numberoffiles);
			if (numberoffiles == 0){
				break;
			}
			while (token != NULL){
				token = strtok( NULL, " \n" );
				if (token != NULL){
					filecounter++;
					printf("%s\n", token);
				}
			}
			//filecounter++;
			 
			int t;
                        for (t = 0; t < 63; t++){
                                if (rBufferSave[t] == '\n'){
                                        t++;
                                        break;
                                }
                        }
			//printf("saveeeme(%d)-t(%d)\n",  size, t);
			if (numberoffiles == t){
				//printf("not equal(%s)\n", rBuffer);
				break;
			}else{
				char *recvBuffer = (char *) malloc(sizeof(char) * 64);
				if (recvBuffer == NULL){
					
				}
				strncpy(recvBuffer, rBuffer+t, sizeof(rBuffer)-t-1);
				printf("%s", recvBuffer);
				 
			}
			parseAction = 1;
			
		}else{
			// count \n
			int t;
			for (t = 0; t < 63; t++){
				if (rBuffer[t] == '\n'){
					filecounter++;
				}
			}
			
			printf("%s", rBuffer);
			//filecounter++;
			//printf("%d-%d\n", numberoffiles, filecounter);
			if (numberoffiles == filecounter){
				break;
			}
		}
		memset(rBuffer, 0, sizeof(rBuffer));
	}
	close(Socket);
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
