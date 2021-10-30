#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "record.h"
#include "writeData.h"
#include "Bplus.h"

/*
	argv[1] is table name
	argv[2] is key
*/

int main(int argc, char *argv[])
{
	char database[100],indexbase[100],recbase[100],freebase[100];
	strcpy(database,argv[1]);
	strcat(database,"_data");
	
	strcpy(indexbase,argv[1]);
	strcat(indexbase,"_index");
	
	strcpy(freebase,argv[1]);
	strcat(freebase,"_free");
		
	strcpy(recbase,argv[1]);
	strcat(recbase,"_rec");

	strcat(argv[2],"\n");	
	long int recLoc = searchKey(argv[1],argv[2]);
	if(recLoc==-1)
	{
		printf("99999\n");
		return 0;
	}
	struct record *rec = readRec(recbase, recLoc,1);
	if(!rec->del)
	{
		char *retd = readData(database,rec->offset,rec->length);
		printf("%s\n",retd);
		free(retd);
	}
	else
	{
		printf("-1\n");
	}
	
	return 0;
}
