#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "record.h"


struct record *readRecAll(char *fileName)
{	
	FILE *recf;
	int recn;
	recf = fopen(fileName , "r+");
	int fs = fseek(recf, 0, SEEK_SET);
	fread(&recn,4,1,recf);
	struct record *inData = (struct record *)malloc(sizeof(struct record) * recn);
	fs = fread(inData,sizeof(struct record),recn, recf);
	fclose(recf);
	return inData;
}

void writeRecAll(char *fileName,struct record *outData, int recn)
{
	FILE *recf;
	recf = fopen(fileName , "w+");
	int fs = fseek(recf, 0, SEEK_SET);
	fwrite(&recn,sizeof(int),1, recf);
	fwrite(outData,sizeof(struct record),recn, recf);
	fclose(recf);
}

struct record *readRec(char *fileName,long int recLoc, int recCnt)
{	
	FILE *recf;
	//int recn;
	recf = fopen(fileName , "r+");
	int fs = fseek(recf, recLoc, SEEK_SET);
	//fread(&recn,4,1,recf);
	struct record *inData = (struct record *)malloc(sizeof(struct record) * recCnt);
	fs = fread(inData,sizeof(struct record),recCnt, recf);
	fclose(recf);
	return inData;
}

void writeRec(char *fileName,struct record *outData, long int recLoc,int recCnt)
{
	FILE *recf =NULL;
	recf = fopen(fileName , "r+");
	if(recf ==NULL)
	{	
		recf=fopen(fileName,"a+");
	}
	int fs = fseek(recf, recLoc, SEEK_SET);
	//fwrite(&recn,sizeof(int),1, recf);
	fwrite(outData,sizeof(struct record),recCnt, recf);
	fclose(recf);
}

long int appendRec(char *filename, struct record *outData, int recCnt)
{
	FILE *recf;
	recf = fopen(filename,"r+");
	if(recf ==NULL)
	{	
		recf=fopen(filename,"a+");
	}
	fseek(recf, 0, SEEK_END);
	long int sz = ftell(recf);
	fwrite(outData,sizeof(struct record),recCnt,recf);
	fclose(recf);
	return sz;
}


void deleteRec(char *filename,long int recLoc)
{	
	FILE *recf;
	recf = fopen(filename,"r+");
	if(recf ==NULL)
	{	
		recf=fopen(filename,"a+");
	}
	fseek(recf, 0, SEEK_END);
	long int fz = ftell(recf);
	long int locBeg,locEnd;
	if(fz-sizeof(struct record)==recLoc)
	{
		struct record *inData = (struct record *)malloc(sizeof(struct record));
		fseek(recf, recLoc, SEEK_SET);	
		fread(inData,sizeof(struct record),1,recf);
		inData->del = true;	
		fseek(recf, recLoc, SEEK_SET);
		fwrite(inData,sizeof(struct record),1,recf);
		fclose(recf);
		locEnd = inData->offset + inData->length-1;
		locBeg = inData->offset;
		free(inData);
	}
	else
	{ 
		fseek(recf, recLoc, SEEK_SET);
		struct record *inData = (struct record *)malloc(sizeof(struct record)*2);
		fread(inData,sizeof(struct record),2,recf);
		inData[0].del = true;
		fseek(recf, recLoc, SEEK_SET);
		fwrite(inData,sizeof(struct record),1,recf);
		fclose(recf);
		locEnd = inData[1].offset-1;
		locBeg = inData[0].offset;
		free(inData);
	}
	char empfname[100];
	strcpy(empfname,filename);
	strcat(empfname,"_emp");
	FILE *empf;
	empf = fopen(empfname,"r+");	
	if(empf ==NULL)
	{	
		empf = fopen(empfname,"a+");
		long int cnt = 1;
		fwrite(&cnt,8,1,empf);
		//int *earr = (int *)malloc(sizeof(int) * cnt * 2);
		fwrite(&locBeg,8,1,empf);	
		fwrite(&locEnd,8,1,empf);
		printf("4444444444444\n");
		fclose(empf);
	}
	else
	{
		long int cnt;
		fread(&cnt,8,1,empf);
		fseek(empf,0,SEEK_SET);
		cnt++;
		fwrite(&cnt,8,1,empf);
		fseek(empf,0,SEEK_END);
		
		fwrite(&locBeg,8,1,empf);
		fwrite(&locEnd,8,1,empf);
		fclose(empf);
	}

}

long int fillLoc(char *filename,long int dlen)
{
	
	char empfname[100];
	strcpy(empfname,filename);
	strcat(empfname,"_emp");
	FILE *empf;
	empf = fopen(empfname,"r+");
	if(empf==NULL)
	{
		return -1;
	}
	else
	{
		
		long int cnt;
		fseek(empf,0,SEEK_SET);
		
		fread(&cnt,8,1,empf);
		printf("%ld\n",cnt);
		if(!cnt)
			return -1;
		
		long int *empArr = (long int *)malloc(sizeof(long int) * cnt *2);
		fread(empArr,sizeof(long int),cnt*2,empf);
		int i,j;
		fclose(empf);
		
		for(i=0;i<cnt;i++)
		{
			if(empArr[i*2+1]-empArr[i*2]+1 >= dlen)
			{
				break;
			}
		}
		if(i==cnt)
			return -1;
		long int ret = empArr[i*2];
		empf = fopen(empfname,"w");
		cnt--;
		for(j=i*2;j<cnt*2;j++)
		{
			empArr[j] = empArr[j+2];
		}
		fwrite(&cnt,8,1,empf);
		fwrite(empArr,sizeof(long int),cnt*2,empf);
		fclose(empf);
		return ret;
	}	
}
/*int testMain()
{
	FILE *recf;

	char buffer[409600];
	int recn;
	scanf("%d",&recn);
	struct record *recs = (struct record *)malloc(sizeof(struct record) * recn);
	int i;
	fgets(buffer,409600,stdin);
	for(i=0;i<recn;i++)
	{

		fgets(recs[i].key,50,stdin);
		fgets(buffer,409600,stdin);
		recs[i].offset = atoi(buffer);
		recs[i].del = true; 
		printf("%ld\n",sizeof(recs[i-1]));
	}
	fwrite(&recn,sizeof(int),1, recf);
	fwrite(&recs[0],sizeof(struct record),recn, recf);
	int fs = fseek(recf, 0, SEEK_SET);
	fread(&recn,4,1,recf);
	struct record *reco = (struct record *)malloc(sizeof(struct record) * recn);
	fs = fread(reco,sizeof(struct record),recn, recf);
	for(i=0;i<4;i++)
	{
		printf("%s",reco[i].key);
		printf("%d\n",reco[i].offset);
	}
	return 0;
}*/
