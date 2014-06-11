
CFLAGS=-Werror -Wall -g -O2 -std=gnu99 -I./includes

#LIBS=lib/genlib.a 

SEMFLAGS=-lpthread -lrt -lm


all: run test

clean:
	rm -f includes/commonlib.a  run test includes/config_client.o includes/config_server.o includes/commonlib.o

run: FileServer.c includes/commonlib.a includes/commonlib.h
	gcc $(CFLAGS) FileServer.c -o run $(SEMFLAGS)

test: test.c includes/commonlib.a includes/commonlib.h
	gcc $(CFLAGS) test/client.c -o test $(SEMFLAGS)

includes/config_client.o: includes/config_client.c
	gcc -c $(CFLAGS) includes/config_client.c -o includes/config_client.o $(SEMFLAGS)

includes/config_server.o: includes/config_server.c
	gcc -c $(CFLAGS) includes/config_server.c -o includes/config_server.o $(SEMFLAGS)

includes/clientlib.o: includes/clientlib.c
	gcc -c $(CFLAGS) includes/clientlib.c -o includes/clientlib.o $(SEMFLAGS)

includes/commonlib.o: includes/commonlib.c
	gcc -c $(CFLAGS) includes/commonlib.c -o includes/commonlib.o $(SEMFLAGS)



includes/commonlib.a: includes/config_client.o includes/config_server.o includes/commonlib.o
	ar crs includes/commonlib.a includes/config_client.o includes/config_server.o includes/commonlib.o
