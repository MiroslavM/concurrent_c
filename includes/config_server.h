#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include "commonlib.h"

#define PIDFILE "run.pid"
#define SRVPORT 6666
#define RCVBUFFSIZE 1024
#define MAXCONNECTIONS 32

#ifndef SERVERCONF
#define SERVERCONF

//Fehler werden auf stderr ausgegeben.
void handleErrors(int number, const char *message);

//Manipulation der Dateien
void listFiles();
void createFile(char* fileName, unsigned long fileSize, char* content);
void readFile(char* fileName);
void updateFile(char* fileName, unsigned long fileSize, char* content);
void deleteFile(char* fileName);

//Initialisierung des Daemons
int savePid(int pid);  //Daemon Prozess ID wird gespeichert
int stop();  //Signalbehandlung wenn Benutzer run stop ausführt, sollen die Ressourcen freigegeben werden.
int checkProcessState();

//Initialisierung des TCP-Servers
int initServer();
void signalHandler();

//FileInfo Struktur
struct FileInfo {
  sem_t semaphore;
  int semaphoreId;
  int fileInfoShmid;
  unsigned long fileSize;
  char fileName;
  char content;
  int sharedMemoryId;
  struct FileInfo *prevFile;
  struct FileInfo *nextFile;
};
#endif

