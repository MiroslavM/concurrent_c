#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include "commonlib.h"

#define BUFFSIZE 10240
#define TRUE  1
#define FALSE 0
#define LOGFILE "log.txt"
#define PIDFILE "run.pid"

#ifndef SERVERCONF
#define SERVERCONF

//Manipulation der Dateien
void listFiles();
void createFile(char* fileName, unsigned long fileSize, char* content);
void readFile(char* fileName);
void updateFile(char* fileName, unsigned long fileSize, char* content);
void deleteFile(char* fileName);

int savePid(int pid);  //Daemon Prozess ID wird gespeichert
int stop();  //Signalbehandlung wenn Benutzer run stop ausführt, sollen die Ressourcen freigegeben werden.

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

