#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>

#include "writeData.h"
#include "record.h"
#include "Bplus.h"

int main()
{
	FILE *ytf = fopen("/media/ming/disk1/MingDB/youtube.txt","r");
	long int y = 0;
	char buffer[409600];
	char writeBuffer[2000000];
	int len =strlen("@url:https://www.youtube.com/watch?v=");
	strcpy(writeBuffer,"\0");
	long int n =0,lp=0;
	long int maxl = 0;
	//printf("--%ld\n",searchKey("yt","ZZJxkv9In5E\n"));
	for(y=0;; y++)
	{
	//	break;
		if(fgets(buffer,409600,ytf)==NULL)
			break;
		
		char *ptr = strstr(buffer,"@url:https://www.youtube.com/watch?v=");
		if(ptr!=NULL && y )
		{
			struct record inRec;
			//long int datloc = appendData(writeBuffer,"testYT");
			char *qtr = strstr(ptr+len,"&");
			if(qtr!=NULL)
			{
				strncpy(inRec.key,ptr,qtr-ptr);
				inRec.key[qtr-ptr]='\0';
			}
			else
			{
				strcpy(inRec.key,ptr + len);
			}
			/*if(maxl < strlen(inRec.key))
			{
				maxl = strlen(inRec.key);
			}*/
			inRec.del = false;
			//inRec.length = strlen(writeBuffer);
			//inRec.length += (3 - y % 7); 
			if(n % 100 == 10)
			{
				char writeBuffer2[2000000];
				if(y>=5)
				{
					strcpy(writeBuffer2,"123456789poi");
					strcat(writeBuffer2,writeBuffer);	
				}
				else
				{
					strcpy(writeBuffer2,writeBuffer + 12);
				}
				inRec.length = strlen(writeBuffer2);
				
				deleteRec("testRC", searchKey("yt",inRec.key));	
				long int updLoc = fillLoc("testRC",inRec.length);
				if(updLoc ==-1)
				{	
					printf("555555555\n");
					long int datloc = appendData(writeBuffer,"testYT");
					printf("555555555\n");
					inRec.offset = datloc;
					inRec.del = false;
					long int tloc = appeandRec("testRC",&inRec,1);
					printf("555555555\n");
					updateLoc("yt",inRec.key,tloc);
					printf("555555555\n");
				}
				else
				{
				
					FILE *fd  = NULL;
					fd = fopen("testRC","r+");
					fseek(fd,updLoc,SEEK_SET);
					printf("655555555\n");
					fwrite(writeBuffer,1,inRec.length,fd);
					fclose(fd);
					inRec.offset = updLoc;
					inRec.del = false;
					printf("655555555\n");
					long int tloc = appeandRec("testRC",&inRec,1);
					printf("655555555\n");
					updateLoc("yt",inRec.key,tloc);
					printf("655555555\n");
				}
				
			}

			/*long int sl = searchKey("yt",inRec.key);
			if(sl==-1){
				printf("%d %s\n",sl,inRec.key);
				exit(1);
				lp++;
			}*/
			
			//inRec.offset = datloc;
			//inRec = readRec("testRC",n * sizeof(struct record),1);
			//long int tloc = appeandRec("testRC",&inRec,1);
	//		insertKey("yt",inRec.key,tloc);
			/*if(inRec->offset-datloc)
			{
				printf("%ld %ld\n",inRec->offset ,datloc);
				printf("%s %s\n",inRec->key,ptr+len);
				printf("%s\n",writeBuffer);
				printf("%d %d\n",pp,inRec->offset-oo);
				break;
			}
			oo = inRec->offset;
			free(inRec);
			pp=strlen(writeBuffer);
			strcpy(lastBuf,writeBuffer);*/
			strcpy(writeBuffer,buffer);
			
		//	printf("%ld\n",n);
			continue;
		}
		n++;
		/*if(n > 500000)
			break;*/
		if(strcmp(buffer,"\n")==0 || strcmp(buffer,"@\n")==0)
			continue;
		strcat(writeBuffer,buffer);
	}
	fclose(ytf);
	printf("%ld\n",n);
	printf("%d\n",maxl);
	printf("%d\n",lp);
	return 0;
}	
