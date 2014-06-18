#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
//used for tcp connections
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFSIZE 10240
#define TRUE  1
#define FALSE 0
#define SRVIP "127.0.0.1"
