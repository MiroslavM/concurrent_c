#include "includes/config_server.h"

//FileInfo Struktur
struct FileInfo {
  sem_t semaphore;
  int semaphoreId;
  int fileInfoShmid;
  unsigned long fileSize;
//  char* fileName;
//  char* content;
  int sharedMemoryId;
//  struct* prevFile;
//  struct* nextFile;
} fileInfo;

int main(int argc, char* argv[]){
  //Benötigte Variablen
  //char fileName[256];
  printf("Sein oder nicht sein");
  
  //Starte Daemon
  
  //Oeffne TCP Socket
  
  //Starte Kindprozess für ankommende Clients
  
  //Reagiere auf ankommende Befehle, case switches (list, create, read, update, delete)
  exit(0);
  
  
}
