#include "includes/commonlib.h"
#include "includes/config_server.h"

void listFiles();
void createFile(char* fileName, unsigned long fileSize, char* content);
void readFile(char* fileName);
void updateFile(char* fileName, unsigned long fileSize, char* content);
void deleteFile(char* fileName);

//Signalbehandlung wenn Benutzer run stop ausführt, sollen die Ressourcen freigegeben werden.
void stop();


int main(int argc, char* argv[]){
  
  //Starte Daemon
  
  //Oeffne TCP Socket
  
  //Starte Kindprozess für ankommende Clients
  
  //Reagiere auf ankommende Befehle, case switches (list, create, read, update, delete)
  
  
  
}
