/*-----------------------------------------------------------
 *
 * SWE2007: Software Experiment 2 (FALL 2018)
 *
 * Skeleton code for PA#1
 * 
 * September 17, 2018.
 * Computer Network Lab, Sungkyunkwan University
 *
 * Student Id   : 2017313008
 * Student Name : Kim Tae Eun
 *
 *-----------------------------------------------------------
 */

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

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
	

void indexBuilder(const char* inputFileNm, const char* indexFileNm)
{
	/* Write your own C code */
	
	int fd=open(inputFileNm,O_RDONLY);
	int ifd=open(indexFileNm, O_RDWR | O_CREAT, 0755);
	int now=0, start=0, cntle=0, chap=0, clau=0, pastChap=1,pastClau=1, m=0, k=-1, byte=0, ok=0, numOfChap=0, numOfClau=-1, numOfWord=0, numOfIndex=0, cn=0, ch=0, cl=0, pl=0, id=0, wd=0, kh=0, gh=0;
	char a[2], b[3], c[4], word[30], num[30];
	
	
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
		

		if(numOfChap<chap)
			numOfChap=chap, numOfClau+=pastClau;
		
		

		if(cntle==1){
			for(int i=0;i<2;i++){
				if(a[i]>='A' && a[i]<='z'){ 
					now=1, k++;
					if(a[i]>='A' && a[i]<='Z') a[i]=a[i]+32;
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
						m=0, ok=1, now=0, numOfWord++, gh=0;
					}
				}	
			}
		}
	}
	numOfClau+=clau;
	if(m>0){
		insert(word, pastChap, pastClau, byte,m);
		numOfWord++;
	}

	struct node *horse;
	ok=0;
	char len[4], result[15];
	for(int i=0;i<90000000;i++){
		horse=hashTable[i];
		if(hashTable[i]!=NULL){
			ok=1;
			while(horse!=NULL){
				if(ok==1){
					numOfIndex++;
					write(ifd, horse->name, horse->wordLen);
					write(ifd, ": ",2);
					cn=intToChar(horse->count, len, 0);
					write(ifd, len, cn);
					write(ifd, ", ",2);
					ok=0;
				}
				ch=intToChar(horse->chap,result,0);
				result[ch]=':';
				cl=intToChar(horse->clau,result,ch+1);
				result[ch+cl+1]=':';
				pl=intToChar(horse->place,result,ch+cl+2);
				if(pl==0) pl=1, result[ch+cl+2]='0';
				write(ifd,result, ch+cl+pl+2);
		
				horse=horse->next;

				if(horse!=NULL) write(ifd, ", ",2);
			}
			write(ifd, "\n",1);
		}
	}


	
	num[0]='<';
	ch=intToChar(numOfChap,num,1);
	num[ch+1]='>', num[ch+2]='(';			
	cl=intToChar(numOfClau,num,ch+3);
	num[ch+cl+3]=')', num[ch+cl+4]='{';
	id=intToChar(numOfIndex,num,ch+cl+5);
	num[ch+cl+id+5]='}', num[ch+cl+id+6]='[';
	wd=intToChar(numOfWord,num,ch+cl+id+7);
	num[ch+cl+id+wd+7]=']';
	write(ifd,num, ch+cl+id+wd+8);

	close(ifd);	
	close(fd);
}


