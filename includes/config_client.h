#include "commonlib.h"

#define SRVPORT 6666
#define RCVBUFFSIZE 1024

//Prototypen der Client functionen
#ifndef CLIENTCONF
#define CLIENTCONF

//Funktionen um die Dateien Clientseitig zu manipulieren
void fileList(int Socket);
void fileCreate(int Socket, char *fileName);
void fileRead(int Socket, char *fileName);
void fileUpdate(int Socket, char *localFileName, char *remoteFileName);
void fileDelete(int Socket, char *remoteFileName);

//Resultate vom Server abfangen und auf Terminal ausgeben.
void getListResult(int Socket);
void getCreateResult(int Socket);
void getReadResult(int Socket);
void getUpdateResult(int Socket);
void getDeleteResult(int Socket);

//Überprüfen von Client-Eingaben
int clientCommandParser(int argc, char **argv);
void howToUse(char **argv);
//Für die Überprüfung der Dateinamen-Länge
int checkFilename(char *fileName);

//Funktion welche den TCP Socket bereitstellt.
int initClientConnection();

//Fehlerbehandlungsroutine, siehe Server
void handleErrors(int number, const char *message);

#endif
