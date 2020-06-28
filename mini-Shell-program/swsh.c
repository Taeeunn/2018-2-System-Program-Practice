#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>

int token_cmd(const char *str, const char *delim, char ***argvpt);
void run(char *command);
void run_command(char *command);

int pl[2];

int main(void) {

        pid_t childpid=0;
        char buf[256], ch, **str;
        int len, cnt;

        pipe(pl);

        while(1) {
		
                printf("$");

		signal(SIGINT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);

                if(fgets(buf, 256 , stdin) == NULL) continue;

                len = strlen(buf);

                if(buf[len-1] == '\n') buf[len-1] = 0;

		
                token_cmd(buf, " \t", &str);

		if(str[0]=='\0') continue;

		if(!strcmp(str[0], "exit")) break;

		if(str[1]=='\0'){
			if(!strcmp(str[0], "pwd")){
				char location[100];
				getcwd(location,100);
				printf("%s\n",location);
				continue;
			}
			else if(!strcmp(str[0], "cd")){
				chdir(getenv("HOME"));
				continue;
			}
		}
			
		else if(str[1]!='\0' && str[2]=='\0' && strcmp(str[1], "--help")){
			if(!strcmp(str[0], "cat")){
				int fd=open(str[1], O_RDONLY);
				if(fd<0){
					printf("cat: %s: 그런 파일이나 디렉터리가 없습니다\n", str[1]);
					continue;
				}
				else if(opendir(str[1])){
					printf("cat: %s: 디렉터리입니다\n", str[1]);
					continue;
				}
				while(read(fd, &ch, 1)) write(1, &ch, 1);
				close(fd);
				continue;
			}
			
			else if(!strcmp(str[0],"rm")){
				int fd=open(str[1], O_RDONLY);
				if(fd<0){
					printf("rm: %s: 그런 파일이나 디렉터리가 없습니다\n", str[1]);
					continue;
				}
				if(opendir(str[1])){
					printf("rm: '%s'를 지울 수 없음: 디렉터리입니다\n", str[1]);
					continue;
				}
				unlink(str[1]);
				continue;
			}
			else if(!strcmp(str[0],"head")){
				char buf[128];
				for(int j=0;j<128;j++) buf[j]='\0';
				FILE *fp=fopen(str[1], "r");
				if(opendir(str[1])){
					printf("head: '%s'을(를) 읽는 도중 오류 발생: 디렉터리입니다\n", str[1]);
					continue;
				}
				if (fp==NULL){
					printf("head: %s: 그런 파일이나 디렉터리가 없습니다\n", str[1]);
					continue;
				}	
				for(int i=0;i<10;i++){
					fgets(buf, 128, fp);
					printf("%s",buf);
					for(int j=0;j<128;j++) buf[j]='\0';	
				}
				fclose(fp);
				continue;
			}
			else if(!strcmp(str[0],"tail")){
				FILE *fp=fopen(str[1], "r");
				if(opendir(str[1])){
					printf("tail: '%s'을(를) 읽는 도중 오류 발생: 디렉터리입니다\n", str[1]);
					continue;
				}
				if (fp==NULL){
					printf("tail: %s: 그런 파일이나 디렉터리가 없습니다\n", str[1]);
					continue;
				}
				fseek(fp, 0, SEEK_END);
				int fsize=ftell(fp), line=0, ch;
				for(int i=1;i<=fsize;i++){
					fseek(fp, -i, SEEK_END);
					ch=fgetc(fp);
					if(ch=='\n') line++;
				}
				cnt=11;
				if(line<cnt){
					fseek(fp, 0, SEEK_SET);
					for(int i=0;i<fsize;i++){
						ch=fgetc(fp);
						putchar(ch);
					}
					fclose(fp);
					continue;
				}
				
				for(int i=1;i<=fsize;i++){
					fseek(fp, -i, SEEK_END);
					ch= fgetc(fp);
					if(ch=='\n'){
						cnt--;
						if(cnt==0){
							for(int j=i;j>1;j--){
								ch=fgetc(fp);
								putchar(ch);
							}
							break;
						}
					}	
				}
				fclose(fp);
				continue;
			}
	
		} 
		else if(str[1]!='\0' && str[2]!='\0' && str[3]=='\0' && strcmp(str[1], "--help")){
			if(!strcmp(str[0], "cp")){
				if(!opendir(str[2])){
					FILE *src=fopen(str[1], "r");
					if(src==NULL) {
						printf("cp: %s: 그런 파일이나 디렉터리가 없습니다\n", str[1]);
						continue;
					}
					if(opendir(str[1])){
						printf("cp: '%s' 디렉터리 생략 중\n", str[1]);
						continue;
					}
					FILE *dst=fopen(str[2], "w");
				        
					while(!feof(src)){
						ch=(char) fgetc(src);
						if(ch!=EOF)
							fputc((int)ch, dst);
					}
					fclose(src);
					fclose(dst);
	
					continue;
				}
				
			}
		
			if(!strcmp(str[0], "mv")){
				if(!opendir(str[1]) && !opendir(str[2])){
					FILE *src=fopen(str[1], "r");
					if(src==NULL) {
						printf("mv: '%s'를 설명할 수 없음: 그런 파일이나 디렉터리가 없습니다\n", str[1]);
						continue;
					}
					FILE *dst=fopen(str[2], "w");
				
					while(!feof(src)){
						ch=(char) fgetc(src);
						if(ch!=EOF)
							fputc((int)ch, dst);
					}
					fclose(src);
					fclose(dst);
				 
					unlink(str[1]);
					continue;
				}
			}

		}

		if(!strcmp(str[0],"cd")) {
		
			if(!strcmp(str[1], "~") || !strcmp(str[1], "$HOME")){
				chdir(getenv("HOME"));
				continue;
			}
			if(opendir(str[1])){
                        	chdir(str[1]);
				continue;
			}
			if(!strcmp(str[1], "--help")){
				printf("cd: 사용법: cd [dir]\n");
				continue;
			}
			int fd=open(str[1], O_RDONLY);
			if(fd>0){
				printf("cd: %s: 디렉터리가 아닙니다\n", str[1]);
				continue;
			}
			if(!opendir(str[1])){
				printf("cd: %s: 그런 파일이나 디렉터리가 없습니다\n", str[1]);
				continue;
			}
                }

                write(pl[1], " ", 1);

                if((childpid = fork()) == -1) {
                        perror("Failed to fork child to execute command");
                }
		

                if(childpid == 0) {
                        run_command(buf);
                        return 1;
                }
		
		
		waitpid(childpid, NULL, 0);
		
		if(strchr(buf, '|')!=NULL) sleep(1);

                while(waitpid(-1, NULL, WNOHANG) >0 );
        }
	
        return 0;
}


int token_cmd(const char *str, const char *delim, char ***argvpt) {

        int tokenum=0;
        const char *new;
        char *ch;

        if((str==NULL) || (delim==NULL) || (argvpt==NULL)) return -1;
        

        *argvpt = NULL;

        new = str+strspn(str, delim);

        if((ch = (char *)malloc(strlen(new) + 1)) == NULL) return -1;

        strcpy(ch, new);


        if(strtok(ch, delim) != NULL) 
                for(tokenum = 1; strtok(NULL, delim) != NULL; tokenum++);



        if((*argvpt = malloc((tokenum +1)*sizeof(char *))) == NULL) {
                free(ch);
                return -1;
        }


        if(tokenum == 0) free(ch);

        else{
                strcpy(ch, new);
                **argvpt = strtok(ch, delim);
                for(int i=1; i<tokenum; i++)
                        *((*argvpt) + i) = strtok(NULL, delim);
        }

        *((*argvpt) + tokenum) = NULL;

        return tokenum;
}



void run(char *command) {

        char **argv;

        if(token_cmd(command, " \t", &argv) <= 0)
                fprintf(stderr, "Failed to parse command line\n");

        else{
        	for(int i=0; argv[i] != 0; i++) {
                        for(int j=0; argv[i][j] != 0; j++){
				write(pl[1], &argv[i][j], 1);
                        }
                        write(pl[1], " ", 1);
                }
		
                execvp(argv[0], argv);
        }
}



void run_command(char *command) {

        int pid, cnt, i, fd[2];
        char **pipeline;

        cnt = token_cmd(command, "|", &pipeline);


        if(cnt<=0) {
                fprintf(stderr, "Failed\n");
                exit(1);
        }
	
        for(i=0; i<cnt-1; i++) {

                if(pipe(fd)<0) exit(1);

                else if((pid = fork()) <0) exit(2);

                else if(pid) {
                        if(dup2(fd[1], 1) == -1) exit(1);

                        if(close(fd[0]) || close(fd[1])) exit(1);

                        run(pipeline[i]);
			
                        exit(1);
                }

                if(dup2(fd[0], 0) == -1) exit(1);

                if(close(fd[0]) || close(fd[1])) exit(1);
        }


        run(pipeline[i]);
	
        exit(1);
}
