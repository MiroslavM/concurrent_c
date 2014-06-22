#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include "commonlib.h"

#define PIDFILE "run.pid"
#define SRVPORT 6666
#define MAXCONNECTIONS 32

#ifndef SERVERCONF
#define SERVERCONF

//Fehler werden auf stderr ausgegeben.
void handleErrors(int number, const char *message);

//Manipulation der Dateien
void listFiles(int socket);
void createFile(int socket, char *content, char *fileName, char *fileSize);
void readFile(int socket, char *fileName);
void updateFile(int socket, char *fileName, char *newFileName, char *fileSize, char* content);
void deleteFile(int socket, char *fileName);

//Manipulation von Speicher
int getShmId();
void start();
void append(int semaphoreId, unsigned int fileSize, char fileName, int sharedMemoryId);

//Initialisierung des Daemons
int savePid(int pid);  //Daemon Prozess ID wird gespeichert
int stop();  //Signalbehandlung wenn Benutzer run stop ausführt, sollen die Ressourcen freigegeben werden.
int checkProcessState();

//Initialisierung des TCP-Servers
int initServer();
void signalHandler();
void doProcessing(int socket);

//FileInfo Struktur
struct FileInfo {
  sem_t semaphore;
  int semaphoreId;
  unsigned int fileSize;
  char fileName;
  int sharedMemoryId;
  struct FileInfo *prevFile;
  struct FileInfo *nextFile;
};
#endif

