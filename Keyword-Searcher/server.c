#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 500000

void keywordsearcher(char * key, int keylen, int connfd);
void insert(char str[], int chap, int clau, int place, int len);


int main (int argc, char *argv[]) {
	int n, listenfd, connfd, caddrlen;
	struct hostent *h;
        struct sockaddr_in saddr, caddr;
       	char buf[MAXLINE];
        int port = atoi(argv[1]);  

        if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        	printf("socket() failed.\n");
                exit(1);
        }
        bzero((char *)&saddr, sizeof(saddr));
        saddr.sin_family = AF_INET;
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
        saddr.sin_port = htons(port);   
        if (bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        	printf("bind() failed.\n");
                exit(2);
        }
        if (listen(listenfd, 5) < 0) {
        printf("listen() failed.\n");
        exit(3);
        }

	printf("Started server: %d\n\n",port);  
	int fd=open("genesis.txt",O_RDONLY);
	int now=0, start=0, cntle=0, chap=0, clau=0, pastChap=1,pastClau=1, m=0, k=-1, byte=0, ok=0, kh=0, gh=0;
	char a[2], b[3], c[4], word[30];
	
		
	while(read(fd,a,2)>0){
		if(a[0]=='\n' || a[1]=='\n') start=1, cntle=0, kh=1;
		if(start==1 && 47<a[0] && a[0]<58 && 47<a[1] && a[1]<58){
			chap=(a[0]-'0')*10+(a[1]-'0'), read(fd,a,2);
			if(a[0]==':' && 47<a[1] && a[1]<58){
				cntle=1, start=0, k=-1;
				clau=a[1]-'0', read(fd,a,2);
				if(47<a[0] && a[0]<58 && a[1]==':')
					clau=clau*10+(a[0]-'0');
			}
			else if(47<a[0] && a[0]<58 && a[1]==':'){
				cntle=1, start=0, k=-1;
				chap=chap*10+(a[0]-'0'), read(fd,b,3);
				if(47<b[0] && b[0]<58 && b[1]==':')
					clau=(b[0]-'0');
				else if(47<b[0] && b[0]<58 && 47<b[1] && b[1]<58 && b[2]==':')
					clau=(b[0]-'0')*10+(b[1]-'0');
				else if(47<b[0] && b[0]<58 && 47<b[1] && b[1]<58 && 47<b[2] && b[2]<58)	
					clau=(b[0]-'0')*100+(b[1]-'0')*10+(b[2]-'0');
				lseek(fd,-1, SEEK_CUR);
			}
			else chap=0;
		}
		
		else if(start==1 && 47<a[0] && a[0]<58 && a[1]==':'){
			chap=a[0]-'0', read(fd,a,2);
			if(47<a[0] && a[0]<58 && 47<a[1] && a[1]<58)
				clau=(a[0]-'0')*10+(a[1]-'0'), cntle=1, start=0, k=-1;
			else if(47<a[0] && a[0]<58 && a[1]==':')
				clau=a[0]-'0', cntle=1, start=0, k=-1;
			else chap=0;
		}	

		else if(start==1 && (a[0]=='\n' || a[0]==' ') && 47<a[1] && a[1]<58){
			chap=a[1]-'0', read(fd,a,2);
			if(47<a[0] && a[0]<58 && a[1]==':'){
				cntle=1, start=0, k=-1;
				chap=chap*10+(a[0]-'0'), read(fd,a,2);
				if(47<a[0] && a[0]<58 && a[1]==':')
					clau=a[0]-'0';
				else if(47<a[0] && a[0]<58 && 47<a[1] && a[1]<58)
					clau=(a[0]-'0')*10+(a[1]-'0');
			}
			else if(a[0]==':' && 47<a[1] && a[1]<58){
				cntle=1, start=0, k=-1;
				clau=a[1]-'0', read(fd,a,2);
				if(47<a[0] && a[0]<58 && a[1]==':')
					clau=clau*10+(a[0]-'0');
			}
			else if(47<a[0] && a[0]<58 && 47<a[1] && a[1]<58){
				cntle=1, start=0, k=-1;
				chap=chap*100+(a[0]-'0')*10+(a[1]-'0'), read(fd,c,4);
				if(47<c[1] && c[1]<58 && c[2]==':')
					clau=(c[1]-'0');
				else if(47<c[1] && c[1]<58 && 47<c[2] && c[2]<58 && c[3]==':')
					clau=(c[1]-'0')*10+(c[2]-'0');
				else if(47<c[1] && c[1]<58 && 47<c[2] && c[2]<58 && 47<c[3] && c[3]<58)
					clau=(c[1]-'0')*100+(c[2]-'0')*10+(c[3]-'0');
				lseek(fd,-1, SEEK_CUR);
			}
			else chap=0;
		}

		if(cntle==1){
			for(int i=0;i<2;i++){
				if(a[i]>='A' && a[i]<='z'){ 
					now=1, k++;
					word[m++]=a[i];
					if(ok==1) {
						byte=k, ok=0;
						pastChap=chap, pastClau=clau;
					}
				}
				if(a[i]=='(') {
					if(kh==1 && k==1) k=0,kh=0;
					else k++;
				}
				if(now==1){
					if(a[i]=='\'' || a[i]=='-') word[m++]=a[i], k++, gh++;
					else if(a[i]=='.'||a[i]==','||a[i]==':'||a[i]==';'||a[i]==')'||a[i]=='?'||a[i]=='!') k++, gh++;			
					else if(a[i]==' ' || a[i]=='\n'){
						k++;
						if(kh==1 && byte==2) byte=0, k=m+gh, kh=0;
						if(kh==1 && byte==0) kh=0;
						if(a[i]==' ' && i==0 && a[1]==' ') k++; 
						else if(a[i]==' ' && i==1){
							read(fd,a,2);
							if(a[0]==' ') k++;
							lseek(fd,-2,SEEK_CUR);
						}
						insert(word, pastChap, pastClau, byte,m);
						m=0, ok=1, now=0, gh=0;
					}
				}	
			}
		}
	}
		
	if(m>0) insert(word, pastChap, pastClau, byte,m);

        while (1) {
        	caddrlen = sizeof(caddr);
                if ((connfd = accept(listenfd, (struct sockaddr *)&caddr,(socklen_t *)&caddrlen)) < 0) {
			printf ("accept() failed.\n");
                        continue;
                }

                h = gethostbyaddr((const char *)&caddr.sin_addr.s_addr, sizeof(caddr.sin_addr.s_addr), AF_INET);
              	printf("Connected: %s\n\n", inet_ntoa(*(struct in_addr *)&caddr.sin_addr)); 

		
                while ((n = read(connfd, buf, MAXLINE)) > 0) {
			printf("search: ");
			for(int i=0;i<n-2;i++) printf("%c", buf[i]);
			printf("\n\n");
			keywordsearcher(buf, n-2, connfd);
                }


                printf("Disconnected: %s\n\n", inet_ntoa(*(struct in_addr *)&caddr.sin_addr)); 
                close(connfd);
	}
}
