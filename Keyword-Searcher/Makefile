all: server client

server: server.c keywordsearcher.c
	gcc -o server server.c keywordsearcher.c

client: client.c
	gcc -o client client.c

clean:
	rm -f server
	rm -f client
