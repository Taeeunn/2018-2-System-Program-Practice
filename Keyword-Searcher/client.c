#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

#define MAXLINE 10000

int main (int argc, char *argv[]) {
	int n, cfd;
        struct hostent *h;
        struct sockaddr_in saddr;
        char buf[MAXLINE];
        char *host = argv[1];
        int port = atoi(argv[2]);
        
	if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket() failed.\n"); 
                exit(1);
        }
        if ((h = gethostbyname(host)) == NULL) {
        	printf("invalid hostname %s\n", host); 
                exit(2);
        } 
	 
        bzero((char *)&saddr, sizeof(saddr));
        saddr.sin_family = AF_INET;
        bcopy((char *)h->h_addr, (char *)&saddr.sin_addr.s_addr, h->h_length);
        saddr.sin_port = htons(port);

        if (connect(cfd,(struct sockaddr *)&saddr,sizeof(saddr)) < 0) {
        	printf("connect() failed.\n");
                exit(3);
        }

	write(1,"\n",1);
        while (write(1,"> ",2), (n = read(0, buf, MAXLINE)) > 0) {
		if(buf[0]=='!' && buf[1]=='q') exit(3);
        	write(cfd, buf, n);
		write(cfd, "\n", 1);
                while(n = read(cfd, buf, MAXLINE)){
			write(1, buf, n);
			if(buf[n-1]=='\n' && buf[n-2]=='\n') break;
		}
        }
        close(cfd);
}
