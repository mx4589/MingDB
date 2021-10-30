#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "record.h"
#include "writeData.h"
#include "Bplus.h"

/*
	argv[1] is table name
	argv[2] is data
	argv[3] is key
*/
int main(int argc,char *argv[])
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
	
	long int datloc = appendData(argv[2],database);
	
	struct record inRec;
	inRec.del = false;
	inRec.length = strlen(argv[2]);
	inRec.offset = datloc;
	strcpy(inRec.key,argv[3]);
	
	long int tloc = appendRec(recbase, &inRec,1);
	insertKey(argv[1],inRec.key,tloc);

	
	return 0;
}
