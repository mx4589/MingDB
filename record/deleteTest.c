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
	FILE *ytf = fopen("youtube.txt","r");
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
		//printf("56666\n");
		if(fgets(buffer,409600,ytf)==NULL)
			break;
		
		char *ptr = strstr(buffer,"@url:https://www.youtube.com/watch?v=");
		if(ptr!=NULL && y && (n % 100)==0)
		{
			
		//printf("---6666\n");
			struct record inRec;
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
			if(maxl < strlen(inRec.key))
			{
				maxl = strlen(inRec.key);
			}
		//printf("---6666\n");
			inRec.del = false;
			inRec.length = strlen(writeBuffer);
			//inRec.offset = datloc;
			long int x = searchKey("yt",inRec.key);
		//printf("---6666\n");
			deleteRec("testRC", x);
		//	strcpy(writeBuffer,buffer);
			
		//printf("---76666\n");
			printf("Delete %s\n",inRec.key);
			continue;
		}
		if(strcmp(buffer,"\n")==0 || strcmp(buffer,"@\n")==0)
			continue;
		
			n++;
		//strcat(writeBuffer,buffer);
	}
	fclose(ytf);
	printf("%ld\n",n);
	printf("%d\n",maxl);
	printf("%d\n",lp);
	return 0;
}	
