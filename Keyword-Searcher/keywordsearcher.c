#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

struct node{
	char name[30];
	int chap, clau, place;
	int count, wordLen;
	struct node* next;
};

struct node *hashTable[90000000];

struct node* createNode(char str[], int chap, int clau, int place, int len){
	struct node* newNode=(struct node*)malloc(sizeof(struct node));
	int i;
	for(i=0;i<len;i++) newNode->name[i]=str[i];
	newNode->name[i]='\0';
	newNode->wordLen=len;
	newNode->chap=chap;
	newNode->clau=clau;
	newNode->place=place;

	newNode->next=NULL;

	return newNode;
}

int hashCode(char str[], int len){
	int r=0;
	for(int i=0;i<len;i++) r=r*65599+str[i];
	return r^(r>>16);
}

void insert(char str[], int chap, int clau, int place, int len){
	int hashIndex=hashCode(str,len)%90000000;
	
	struct node *newNode=createNode(str, chap, clau, place, len);
	struct node *n1;
	
	if(hashTable[hashIndex]==NULL){
		hashTable[hashIndex]=newNode;
		hashTable[hashIndex]->count=1;
	}
	else{
		for(n1=hashTable[hashIndex];n1->next!=NULL;n1=n1->next);
		n1->next=newNode;
		hashTable[hashIndex]->count++;
	}
}

int intToChar(int num, char *c, int start){
	int cnt=0, div=1, m=start;
	while((num/div)>0) cnt++, div*=10;
	div/=10;
	for(;cnt>0;cnt--,m++,div/=10){
		c[m]=(num/div)+'0';
		num-=((num/div)*div);	
	}
	
	return m-start;
}
	

void keywordsearcher(char * key, int keylen, int connfd)
{
	int ok=0, ch=0, cl=0, pl=0, two=0, twoplace=0, cntble=0, mdb=0;
	char left[30], right[30], result[15], idx[100000], a, mark1=' ', mark2=' ';
	
	for(int i=0;i<30;i++) left[i]='\0', right[i]='\0';
	key[keylen]='\0';
	
	for(int i=0;i<keylen;i++){
		if(key[i]==' '){
			two=1, twoplace=i;
			break;
		}
	}

	char tmp=key[keylen-1];
	if(tmp=='.' || tmp==',' || tmp==':' || tmp==';' || tmp=='!'|| tmp=='?'|| tmp==')'){
		if(key[keylen-2]==':' || key[keylen-2]==',' || key[keylen-2]==';' || key[keylen-2]=='.' || key[keylen-2]=='?'){
			mdb=1, key[keylen-2]='\0';
		}
		mark2=key[keylen-1];
		key[keylen-1]='\0';
		keylen--;
	}

	if(key[0]=='('){
		for(int i=0;i<keylen-1;i++) key[i]=key[i+1];
		key[keylen-1]='\0', keylen--, mark1='(';
	}

	if(two==1){
		strncpy(left, key, twoplace);
		for(int i=0;i<keylen-twoplace;i++) right[i]=key[i+1+twoplace];
	}
	

	struct node *horse, *hh;
	for(int i=0;i<90000000;i++){
		horse=hashTable[i];
		if(hashTable[i]!=NULL){
			if(!strcmp(key,horse->name)){
				ok=1;
				while(horse!=NULL){
					if(ok==1) strcpy(idx, "genesis.txt"), strcat(idx, "> "), ok=0;
				
					ch=intToChar(horse->chap,result,0);
					result[ch]=':';
					cl=intToChar(horse->clau,result,ch+1);
					result[ch+cl+1]=';';
					pl=intToChar(horse->place,result,ch+cl+2);
					if(pl==0) pl=1, result[ch+cl+2]='0';
					strcat(idx, result);
					
					for(int i=0;i<15;i++) result[i]='\0';
					horse=horse->next;
					if(horse!=NULL) strcat(idx, ",");
				}
			}
			else if(!strcmp(left,horse->name)){
				ok=1;
				while(horse!=NULL){
					hh=hashTable[hashCode(right, keylen-twoplace-1)%90000000];
					cntble=0;
					while(hh!=NULL){
						if(hh->chap==horse->chap && hh->clau==horse->clau && hh->place==horse->place+twoplace+1){
							cntble=1;
							break;
						}
						else hh=hh->next;
					}
					if(cntble==1){
						if(ok==1) strcpy(idx, "genesis.txt"), strcat(idx, "> "), ok=0;
						
						ch=intToChar(horse->chap,result,0);
						result[ch]=':';
						cl=intToChar(horse->clau,result,ch+1);
						result[ch+cl+1]=';';
						pl=intToChar(horse->place,result,ch+cl+2);
						if(pl==0) pl=1, result[ch+cl+2]='0';
						strcat(idx, result);
					
						for(int i=0;i<15;i++) result[i]='\0';
						horse=horse->next;
						if(horse!=NULL) strcat(idx, ",");
					}
					else horse=horse->next;
				}
			}
		}
	}
	
	if(strlen(idx)!=0){
		if(idx[strlen(idx)-1]==',') idx[strlen(idx)-1]='}', idx[strlen(idx)]='\0';
		else idx[strlen(idx)]='}', idx[strlen(idx)]='\0';
	}
	else{
		write(connfd, "\n\n",2);
		return;
	}


	int fd=open("genesis.txt",O_RDONLY);
	int start=0, fn=0, plc=0, new=0, st=0, blk=0, s=8, db=0, cnt=0, vld=0;
	char chcl[20], nextchcl[20], stc[10000];
	
	for(int i=0;i<strlen(idx);i++){
		if(idx[i]==' ') start=1, strcpy(chcl,"genesis:");
		else if(start==1){
			if(fn==0 && ((47<idx[i] && idx[i]<58)|| idx[i]==':')) chcl[strlen(chcl)]=idx[i];
			if(idx[i]==';') fn=1;
			else if(fn==1 && 47<idx[i] && idx[i]<58) plc=plc*10+(idx[i]-'0');
		}
		
		if(idx[i]==',' || idx[i]=='}'){
			chcl[strlen(chcl)]=':', chcl[strlen(chcl)]='\0';
			fn=0, s=8;
			strcpy(nextchcl, "genesis:");
			for(int j=0;j<20;j++){
				nextchcl[s++]=idx[i+j+1];
				if(idx[i+j+2]==';')break;
			}
			nextchcl[s]=':';
			
			lseek(fd,0,SEEK_SET);
			if(db==0) strcpy(stc, "genesis:");
			
			if(strcmp(nextchcl, chcl)){
				if(db==0){
					while(read(fd,&a,1)>0){
						if(a=='\n') {
							new=1, st=0;
							if(!strncmp(chcl,stc,strlen(chcl))) {
								blk=0, stc[strlen(stc)]='\n';
								while(stc[blk++]!=' '){}

								if(stc[blk+plc-1]==mark1 && stc[blk+plc+keylen]==mark2){
									if(mark2!=' ') keylen++;
									for(int i=strlen(stc)-1;i>=blk+plc;i--) stc[i+1]=stc[i];
									if (stc[blk+plc-1]=='(') stc[blk+plc-1]='[', stc[blk+plc]='(';
									else stc[blk+plc]='[';
									for(int i=strlen(stc)-1;i>=blk+plc+keylen+1;i--) stc[i+1]=stc[i];
									if(mdb==1 && stc[blk+plc+keylen+2]==')') stc[blk+plc+keylen+1]=')', stc[blk+plc+keylen+2]=']';
									else stc[blk+plc+keylen+1]=']';
									if(mark2!=' ') keylen--;
								}
								break;
							}
							else{
								for(int i=8;i<10000;i++) stc[i]='\0';
							}
	
						}
						if(new==1 && 47<a && a<58) st=1;
						if(st==1) stc[strlen(stc)]=a;
					}
				}
				else{
					cnt=0;
					for(int i=0;i<strlen(stc);i++){
						if(stc[i]=='[' || stc[i]==']') cnt++;
					}
					plc+=cnt;
				
					if(stc[blk+plc-1]==mark1 && stc[blk+plc+keylen]==mark2){
						if(mark2!=' ') keylen++;
						for(int i=strlen(stc)-1;i>=blk+plc;i--) stc[i+1]=stc[i];
						if (stc[blk+plc-1]=='(') stc[blk+plc-1]='[', stc[blk+plc]='(';
						else stc[blk+plc]='[';
						for(int i=strlen(stc)-1;i>=blk+plc+keylen+1;i--) stc[i+1]=stc[i];
						if(mdb==1 && stc[blk+plc+keylen+2]==')') stc[blk+plc+keylen+1]=')', stc[blk+plc+keylen+2]=']';
						else stc[blk+plc+keylen+1]=']';
						if(mark2!=' ') keylen--;
					}
				}

				db=0, cnt=0;
				
				for(int i=0;i<strlen(stc);i++){
					if(stc[i]=='['){
						vld=1;
						break;
						
					}
				}

				if(vld==1 && idx[i]=='}') stc[strlen(stc)]='\n';
				if(vld==0 && idx[i]=='}') write(connfd, "\n\n",2);
				if(vld==1) write(connfd, stc, strlen(stc)), vld=0;
				for(int i=8;i<20;i++) nextchcl[i]='\0';
			}
			else{
				cnt=0;
				if(db==1) {
					for(int i=0;i<strlen(stc);i++){
						if(stc[i]=='[' || stc[i]==']') cnt++;
					}
					plc+=cnt;
				}

				while(read(fd,&a,1)>0){
					if(a=='\n') {
						new=1, st=0;
					
						if(!strncmp(chcl,stc,strlen(chcl))) {
							blk=0;
							if(db==0) stc[strlen(stc)]='\n';
							if(idx[i]=='}') stc[strlen(stc)]='\n';
							while(stc[blk++]!=' '){}

							if(stc[blk+plc-1]==mark1 && stc[blk+plc+keylen]==mark2){
								if(mark2!=' ') keylen++;
								for(int i=strlen(stc)-1;i>=blk+plc;i--) stc[i+1]=stc[i];
								if (stc[blk+plc-1]=='(') stc[blk+plc-1]='[', stc[blk+plc]='(';
								else stc[blk+plc]='[';
								for(int i=strlen(stc)-1;i>=blk+plc+keylen+1;i--) stc[i+1]=stc[i];
								if(mdb==1 && stc[blk+plc+keylen+2]==')') stc[blk+plc+keylen+1]=')', stc[blk+plc+keylen+2]=']';
								else stc[blk+plc+keylen+1]=']';
								if(mark2!=' ') keylen--;
							}
							break;
						}
						else{
							for(int i=8;i<10000;i++) stc[i]='\0';
						}
	
					}
					if(new==1 && 47<a && a<58) st=1;
					if(st==1) stc[strlen(stc)]=a;
				}
				db=1;
			}

			for(int i=8;i<20;i++) chcl[i]='\0';
			if(db==0) for(int i=0;i<10000;i++) stc[i]='\0';
			plc=0;
		} 
	}
	for(int i=0;i<100000;i++) idx[i]='\0';	
	mark1=' ', mark2=' ', mdb=0;
}


