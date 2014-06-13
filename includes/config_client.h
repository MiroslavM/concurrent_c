#include "commonlib.h"

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
#endif
