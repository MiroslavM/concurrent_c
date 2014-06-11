#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#define BUFFSIZE 10240
#define TRUE  1
#define FALSE 0

struct FileInfo{
  sem_t semaphore;
  int semaphoreId;
  int shmid;
  unsigned long fileSize;
  char fileName[256];
  char *content;
  int sharedMemoryIdContent;
  struct *prevFile;
  struct *nextFile;
};
