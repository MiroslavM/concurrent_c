#include "commonlib.h"

#define SRVPORT 6666
#define RCVBUFFSIZE 1024

//Prototypen der Client functionen
#ifndef CLIENTCONF
#define CLIENTCONF

//Funktionen um die Dateien Clientseitig zu manipulieren
void fileList(int Socket);
void fileCreate(int Socket, char *filename);
void fileRead(int Socket, char *filename);
void fileUpdate(int Socket, char *localfilename, char *remotefilename);
void fileDelete(int Socket, char *remotefilename);
//Für die Überprüfung der Dateinamen-Länge
int checkFilename(char *filename);
//Resultate vom Server abfangen und auf Terminal ausgeben.
void getListResult(int Socket);
void getCreateResult(int Socket);
void getReadResult(int Socket);
void getUpdateResult(int Socket);
void getDeleteResult(int Socket);
//Überprüfen von Client-Eingaben
void clientCommandParser(int argc, char **argv);
void howToUse(char **argv);
//Funktion welche den TCP Socket bereitstellt.
int initClientConnection();
//Fehlerbehandlungsroutine, siehe Server
void handleErrors(int number, const char *message);

#endif
