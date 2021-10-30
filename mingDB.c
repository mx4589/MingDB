#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "record.h"
#include "writeData.h"
#include "Bplus.h"
#include "mingDB.h"

void insert(char *command[])
{
	char database[100],indexbase[100],recbase[100],freebase[100];
	strcpy(database,command[0]);
	strcat(database,"_data");
	
	strcpy(indexbase,command[0]);
	strcat(indexbase,"_index");
	
	strcpy(freebase,command[0]);
	strcat(freebase,"_free");
		
	strcpy(recbase,command[0]);
	strcat(recbase,"_rec");
	
	long int datloc = appendData(command[2],database);
	
	struct record inRec;
	inRec.del = false;
	inRec.length = strlen(command[2]);
	inRec.offset = datloc;
	strcpy(inRec.key,command[1]);
	
	long int tloc = appendRec(recbase, &inRec,1);
	insertKey(command[0],inRec.key,tloc);

}

/*
   command[1] is table name
   command[2] is key
 */
void delete(char *command[])
{
	char database[100],indexbase[100],recbase[100],freebase[100];
	strcpy(database,command[0]);
	strcat(database,"_data");

	strcpy(indexbase,command[0]);

	strcpy(freebase,command[0]);
	strcat(freebase,"_free");

	strcpy(recbase,command[0]);
	strcat(recbase,"_rec");
	long int kloc = searchKey(indexbase,command[1]);
	if(kloc < 0)
		return;
	deleteRec(recbase, kloc);

}

void update(char *command[])
{
	char database[100],indexbase[100],recbase[100],freebase[100];
	strcpy(database,command[0]);
	strcat(database,"_data");

	strcpy(indexbase,command[0]);
	//strcat(indexbase,"_index");

	strcpy(freebase,command[0]);
	strcat(freebase,"_free");

	strcpy(recbase,command[0]);
	strcat(recbase,"_rec");
	long int kloc = searchKey(indexbase,command[1]);
	if(kloc < 0)
		return;
	deleteRec(recbase, kloc);

	long int len = strlen(command[2]);
	//printf("%ld\n",len);
	long int updLoc = fillLoc(recbase,len);
	//printf("%ld\n",updLoc);
	struct record inRec;
	if(updLoc == -1)
	{
		long int datloc = appendData(command[2],database);
		inRec.offset = datloc;
		strcpy(inRec.key,command[1]);
		inRec.del = false;
		inRec.length = len;
		long int rloc = appendRec(recbase,&inRec,1);
		updateLoc(indexbase,inRec.key,rloc);
	}
	else
	{

		FILE *fd  = NULL;
		fd = fopen(database,"r+");
		fseek(fd,updLoc,SEEK_SET);
		fwrite(command[2],1,len,fd);
		fclose(fd);
		strcpy(inRec.key,command[1]);
		inRec.offset = updLoc;
		inRec.del = false;
		inRec.length = len;
		long int tloc = appendRec(recbase,&inRec,1);
		updateLoc(indexbase,inRec.key,tloc);
	}



//	return 0;
}

char *get(char *command[])
{
	char database[100],indexbase[100],recbase[100],freebase[100];
	strcpy(database,command[0]);
	strcat(database,"_data");
	
	strcpy(indexbase,command[0]);
	strcat(indexbase,"_index");
	
	strcpy(freebase,command[0]);
	strcat(freebase,"_free");
		
	strcpy(recbase,command[0]);
	strcat(recbase,"_rec");

	//strcat(command[2],"\n");	
	long int recLoc = searchKey(command[0],command[1]);
	if(recLoc==-1)
	{
		printf("99999\n");
		return 0;
	}
	struct record *rec = readRec(recbase, recLoc,1);
	if(!rec->del)
	{
		char *retd = readData(database,rec->offset,rec->length);
		//printf("%s\n",retd);
		//free(retd);
		return retd;
	}
	else
	{
		printf("-1\n");
	}
	
	return 0;
}
