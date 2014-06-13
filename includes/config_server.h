#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include "commonlib.h"

#define BUFFSIZE 10240
#define TRUE  1
#define FALSE 0

#ifndef SERVERCONF
#define SERVERCONF
//Manipulation der Dateien
void listFiles();
void createFile(char* fileName, unsigned long fileSize, char* content);
void readFile(char* fileName);
void updateFile(char* fileName, unsigned long fileSize, char* content);
void deleteFile(char* fileName);
//Daemon Prozess ID wird gespeichert
void savePid(int pid);
//Signalbehandlung wenn Benutzer run stop ausführt, sollen die Ressourcen freigegeben werden.
void stop();
#endif

