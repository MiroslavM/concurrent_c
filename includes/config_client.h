#include "commonlib.h"

#define SRVPORT 6666
#define RCVBUFFSIZE 1024

//Prototypen der Client functionen
#ifndef CLIENTCONF
#define CLIENTCONF

void fileList(int Socket);
void fileCreate(int Socket, char *filename);
void fileRead(int Socket, char *filename);
void fileUpdate(int Socket, char *localfilename, char *remotefilename);
void fileDelete(int Socket, char *remotefilename);


void getListResult(int Socket);
void getCreateResult(int Socket);
void getReadResult(int Socket);
void getUpdateResult(int Socket);
void getDeleteResult(int Socket);

void clientCommandParser(int argc, char **argv);
int initClientConnection();
void handleErrors(int number, const char *message);

#endif
