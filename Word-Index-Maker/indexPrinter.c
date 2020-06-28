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

int intToChar(int num, char *c, int start);

void indexPrinter(const char* indexFileNm, const char* outputFileNm)
{
	/* Write your own C code */

	int ifd=open(indexFileNm,O_RDONLY);
	int ofd=open(outputFileNm, O_RDWR | O_CREAT, 0755);
	
	int len=0, i=0, m=0, ok=0, stop=0, size=0;
	char a[2], bibleNm[30], result[30];

	while(indexFileNm[i++]!='\0') len++;
	for(i=0;i<len-6;i++) bibleNm[i]=indexFileNm[i];
	
	
	write(ofd,bibleNm,len-6);
	write(ofd,": ",2); 


	while(read(ifd,a,2)>0){
		for(i=0;i<2;i++){
			if(a[i]=='<') ok=1;
			else if(ok==1 && (a[i]=='('||a[i]=='{'||a[i]=='[')) result[m++]=' ';
			else if(ok==1 && 47<a[i] && a[i]<58) result[m++]=a[i];
			else if(ok==1 && a[i]==']') break;	
		}
	}
	result[m]='\n';
	write(ofd, result, m+1);


	lseek(ifd,0,SEEK_SET);

	char *buf=(char *)malloc(sizeof(char)*1000);
	
	while(read(ifd,a,2)>0) {
		for(i=0;i<2;i++)
			if(a[i]=='<') stop=1;
			else buf[size++]=a[i];
		if(stop==1) break;
		if(size==1000){
		 	write(ofd,buf,size);
			size=0;
		}
	}
	write(ofd,buf,size);

	close(ifd);
	close(ofd);
}
