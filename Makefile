CC=gcc
CFLAGS=-Wall 

output: askisi3.c 
	$(CC) $(CFLAGS) -o output askisi3.c -lssl -lcrypto 
clean:
	rm -f output askisi3.c
