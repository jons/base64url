
all:
	$(CC) -ansi -g -O0 -Wall codec.c test.c -o test-base64url
	$(CC) -ansi -g -O0 -Wall gen-dtab.c -o gen-dtab
